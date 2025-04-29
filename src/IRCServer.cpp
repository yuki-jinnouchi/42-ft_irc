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
  std::cout << "Port: " << port_ << ", Password: " << password_ << std::endl;
}

IRCServer::~IRCServer() {
  // ソケットを閉じる
  for (std::set<int>::iterator it = listenSocketFds_.begin();
       it != listenSocketFds_.end(); ++it) {
    std::cout << "Shutdown IRC Server: socket fd: " << *it << std::endl;
    // listenSocketFds_の全てのソケットを閉じる
    if (close(*it) == -1) {
      std::cerr << "close failed" << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  listenSocketFds_.clear();
  std::cout << "Server stopped." << std::endl;
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
    std::cerr << "Listening on port: " << port_ << ", fd: " << sockfd << " ("
              << ai->ai_family << ")..." << std::endl;

    // epollで監視
    listenSocketFds_.insert(sockfd);
    struct epoll_event ev;
    ev.events = EPOLLIN; /* Only interested in input events */
    ev.data.fd = sockfd;
    if (epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
      std::cerr << "epoll_ctl failed" << std::endl;
    }
  }
  freeaddrinfo(res);  // アドレス情報の解放
  if (listenSocketFds_.empty()) {
    std::cerr << "Error: socket/bind/listen failed" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void IRCServer::acceptConnection(int listenSocketFd) {
  sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  int sockfd = accept(listenSocketFd, (struct sockaddr*)&addr, &addrlen);
  ClientSession client(sockfd);

  // クライアントのソケットをepollで監視
  struct epoll_event ev;
  ev.events = EPOLLIN; /* Only interested in input events */
  ev.data.fd = sockfd;
  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    std::cerr << "epoll_ctl failed" << std::endl;
  }

  // TODO 後で消す
  // 日付を返す
  time_t t;
  time(&t);
  char* timestr = ctime(&t);
  // write(sockfd, timestr, strlen(timestr));
  send(client.getSocketFd(), timestr, strlen(timestr), 0);
}

void IRCServer::run() {
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
    std::cout << "Ready: " << ready << std::endl;

    for (int j = 0; j < ready; j++) {
      if (evlist[j].events & EPOLLIN) {
        // イベントが発生したソケットに対して処理を行う
        if (listenSocketFds_.count(evlist[0].data.fd)) {
          // 新しい接続を受け入れる
          acceptConnection(evlist[0].data.fd);
        }
      } else if (evlist[j].events & (EPOLLHUP | EPOLLERR)) {
        std::cout << "    closing fd " << evlist[j].data.fd << std::endl;

        if (close(evlist[j].data.fd) == -1) {
          std::cerr << "close failed" << std::endl;
          exit(EXIT_FAILURE);
        }
        listenSocketFds_.erase(evlist[j].data.fd);
      }
    }
  }
}
