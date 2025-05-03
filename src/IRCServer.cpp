#include "IRCServer.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

#include <errno.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "ClientSession.hpp"
#include "CommandHandler.hpp"
#include "IRCLogger.hpp"
#include "IRCMessage.hpp"

#define MAX_BACKLOG 5

IRCServer::IRCServer(const char* port, const char* password) {
  epfd_ = epoll_create1(EPOLL_CLOEXEC);
  if (epfd_ == -1) {
    std::cerr << "epoll_create failed\n";
    exit(EXIT_FAILURE);
  }

  // TODO: ポート番号のバリデーション
  std::istringstream ss(port);
  ss >> port_;

  password_ = std::string(password);

  DEBUG_MSG("Port: " << port_ << ", Password: " << password_);
}

IRCServer::~IRCServer() {
  // listenSockets_の全てのソケットを閉じる
  for (std::map<int, Socket*>::iterator it = listenSockets_.begin();
       it != listenSockets_.end(); ++it) {
    DEBUG_MSG("Shutdown IRC Server: socket fd: " << it->first);
    delete it->second;
  }
  listenSockets_.clear();
  DEBUG_MSG("Server stopped.");
}

IRCServer::IRCServer(const IRCServer& other) {
  *this = other;
}

IRCServer& IRCServer::operator=(const IRCServer& other) {
  if (this == &other) {
    return *this;
  }
  port_ = other.port_;
  password_ = other.password_;
  return *this;
}

void IRCServer::startListen() {
  struct addrinfo hints, *res, *ai;
  memset(&hints, 0, sizeof(hints));
  // TODO IPv6にも対応する必要あり
  // hints.ai_family = AF_UNSPEC;  // IPv4 or IPv6
  hints.ai_family = AF_INET;  // IPv4
  // hints.ai_family = AF_INET6;  // IPv6
  // netcatでIPv6で接続する方法
  // nc -6 ::1 <port>
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;  // 自分のIPアドレスを使用

  int ret = getaddrinfo(NULL, port_.c_str(), &hints, &res);
  if (ret != 0) {
    std::cerr << "Error: getaddrinfo: " << gai_strerror(ret) << std::endl;
    exit(EXIT_FAILURE);
  }
  for (ai = res; ai != NULL; ai = ai->ai_next) {
    int sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd < 0)
      continue;
    // ソケット再利用
    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
      std::cerr << "Error: setsockopt failed" << std::endl;
      close(sockfd);
      continue;
    }
    if (bind(sockfd, ai->ai_addr, ai->ai_addrlen) < 0) {
      close(sockfd);
      continue;
    }
    if (listen(sockfd, MAX_BACKLOG) < 0) {
      close(sockfd);
      continue;
    }
    DEBUG_MSG("Listening on port: " << port_ << ", fd: " << sockfd << " ("
                                    << ai->ai_family << ")...");

    // epollで監視
    listenSockets_[sockfd] = new Socket(sockfd);
    struct epoll_event ev;
    ev.events = EPOLLIN; /* Only interested in input events */
    ev.data.fd = sockfd;
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
      std::cerr << "epoll_ctl failed" << std::endl;
    }
  }
  freeaddrinfo(res);  // アドレス情報の解放
  if (listenSockets_.empty()) {
    std::cerr << "Error: socket/bind/listen failed" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void IRCServer::acceptConnection(int listenSocketFd) {
  sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  int sockfd = accept(listenSocketFd, (struct sockaddr*)&addr, &addrlen);
  clients_[sockfd] = new ClientSession(sockfd);
  DEBUG_MSG("New client connected: " << sockfd
                                     << ", client num: " << clients_.size());

  // クライアントのソケットをepollで監視
  struct epoll_event ev;
  ev.events = EPOLLIN; /* Only interested in input events */
  ev.data.fd = sockfd;
  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    std::cerr << "epoll_ctl failed" << std::endl;
  }
}

void IRCServer::sendResponses(const IRCMessage& msg) {
  for (std::map<ClientSession*, std::string>::const_iterator it =
           msg.getResponses().begin();
       it != msg.getResponses().end(); ++it) {
    it->first->sendMessage(it->second);
  }
}

void IRCServer::run() {
  CommandHandler commandHandler(this);

  while (true) {
    struct epoll_event evlist[EPOLL_MAX_EVENTS];
    int ready = epoll_wait(epfd_, evlist, EPOLL_MAX_EVENTS, -1);
    if (ready == -1) {
      if (errno == EINTR) {
        continue; /* Restart if interrupted by signal */
      } else {
        std::cerr << "epoll_wait failed" << std::endl;
      }
    }
    DEBUG_MSG("Ready: " << ready);

    for (int j = 0; j < ready; j++) {
      if (evlist[j].events & EPOLLIN) {
        // イベントが発生したソケットに対して処理を行う
        if (listenSockets_.count(evlist[0].data.fd) > 0) {
          // 新しい接続を受け入れる
          acceptConnection(evlist[0].data.fd);
        }
        // クライアントを検索
        std::map<int, ClientSession*>::iterator it_from =
            clients_.find(evlist[j].data.fd);
        // クライアントからのデータを受信した場合
        if (it_from != clients_.end()) {
          // 受信したデータを取得
          char buffer[1024];
          ssize_t bytesRead = recv(it_from->first, buffer, sizeof(buffer), 0);
          if (bytesRead > 0) {
            IRCMessage msg(it_from->second, std::string(buffer, bytesRead));
            commandHandler.handleCommand(msg);
            sendResponses(msg);
          } else if (bytesRead == 0) {
            // クライアントが切断された場合
            // クライアントセッションを削除 & ソケットクローズ
            delete it_from->second;
            clients_.erase(it_from);
            DEBUG_MSG("Client disconnected: "
                      << it_from->first << ", client num: " << clients_.size());
          } else {
            std::cerr << "recv failed" << std::endl;
          }
        }

      } else if (evlist[j].events & (EPOLLHUP | EPOLLERR)) {
        DEBUG_MSG("    closing fd " << evlist[j].data.fd);

        if (close(evlist[j].data.fd) == -1) {
          std::cerr << "close failed" << std::endl;
          exit(EXIT_FAILURE);
        }
        listenSockets_.erase(evlist[j].data.fd);
      }
    }
  }
}

std::map<int, ClientSession*>& IRCServer::getClients() {
  return clients_;
}
