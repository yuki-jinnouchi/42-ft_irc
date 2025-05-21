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
#include "Utils.hpp"

IRCServer::IRCServer(const char* port, const char* password) {
  IOWrapper io_;

  // TODO: ポート番号のバリデーション
  std::istringstream ss(port);
  ss >> port_;

  password_ = std::string(password);

  // ログ出力をノンブロッキングに設定
  if (!io_.modify_monitoring(IRCLogger::getInstance().getFd(),
                             EPOLLIN | EPOLLET)) {
    std::cerr << "Error: modify_monitoring failed" << std::endl;
    std::exit(EXIT_FAILURE);
  }

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
    std::exit(EXIT_FAILURE);
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
    // 監視対象に追加
    if (!io_.add_monitoring(sockfd, EPOLLIN)) {
      std::cerr << "Error: add_monitoring failed" << std::endl;
      close(sockfd);
      continue;
    }
    listenSockets_[sockfd] = new Socket(sockfd);
    DEBUG_MSG("Listening on port: " << port_ << ", fd: " << sockfd << " ("
                                    << ai->ai_family << ")...");
  }
  freeaddrinfo(res);  // アドレス情報の解放
  if (listenSockets_.empty()) {
    std::cerr << "Error: socket/bind/listen failed" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void IRCServer::acceptConnection(int listenSocketFd) {
  sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  int sockfd = accept(listenSocketFd, (struct sockaddr*)&addr, &addrlen);
  addClient(new ClientSession(sockfd));

  // クライアントのソケットを監視対象に追加
  if (!io_.add_monitoring(sockfd, EPOLLIN | EPOLLET)) {
    close(sockfd);
    return;
  }
}

void IRCServer::sendResponses(const IRCMessage& msg) {
  for (std::map<ClientSession*, std::string>::const_iterator it =
           msg.getResponses().begin();
       it != msg.getResponses().end(); ++it) {
    if (!io_.sendMessage(it->first, it->second)) {
      disconnectClient(it->first);
    }
  }
}

void IRCServer::disconnectClient(ClientSession* client) {
  int fd = client->getFd();
  // 監視対象から除外
  io_.remove_monitoring(fd);
  // クライアントセッションを削除
  clients_.erase(fd);
  // ソケットクローズ
  delete client;

  DEBUG_MSG("Client disconnected: " << fd
                                    << ", client num: " << clients_.size());
}

void IRCServer::handleClientMessage(int clientFd) {
  // クライアントを検索
  std::map<int, ClientSession*>::iterator it_from = clients_.find(clientFd);
  if (it_from == clients_.end()) {
    return;
  }
  // クライアントからのデータを受信した場合
  char buffer[BUFFER_SIZE];
  ssize_t bytesRead = recv(it_from->first, buffer, sizeof(buffer), 0);
  if (bytesRead == 0) {
    // クライアントが切断された場合
    disconnectClient(it_from->second);
    return;
  }
  if (bytesRead < 0) {
    // recv失敗
    disconnectClient(it_from->second);
    std::cerr << "recv failed. fd: " << it_from->first << std::endl;
    return;
  }
  // bytesRead > 0
  std::string msg =
      it_from->second->popReceivingMsg() + std::string(buffer, bytesRead);
  // CRLFで分割して処理
  std::vector<std::string> split_msgs = Utils::split(msg, "\r\n");

  // 受信途中のメッセージをセッションに退避
  if (!Utils::endsWith(msg, "\r\n")) {
    it_from->second->pushReceivingMsg(split_msgs.back());
    split_msgs.pop_back();
  }

  CommandHandler commandHandler(this);
  for (std::vector<std::string>::iterator it = split_msgs.begin();
       it != split_msgs.end(); ++it) {
    // msgが510(CRLFを含めて512)を超えていたら切断
    if (it->size() > IRCServer::MAX_MSG_SIZE) {
      DEBUG_MSG("Message too long: " << it->size());
      disconnectClient(it_from->second);
      return;
    }
    IRCMessage msg(it_from->second, *it);
    IRCParser::parseRaw(msg);
    commandHandler.handleCommand(msg);
    sendResponses(msg);
  }
  // receiving_msg_が510を超えていたら切断
  if (it_from->second->getReceivingMsg().size() > IRCServer::MAX_MSG_SIZE) {
    DEBUG_MSG(
        "Message too long: " << it_from->second->getReceivingMsg().size());
    disconnectClient(it_from->second);
    return;
  }
}

void IRCServer::resendClientMessage(int clientFd) {
  // クライアントを検索
  std::map<int, ClientSession*>::iterator it_from = clients_.find(clientFd);
  if (it_from == clients_.end()) {
    return;
  }

  if (!io_.sendMessage(it_from->second)) {
    disconnectClient(it_from->second);
  }
}

void IRCServer::run() {
  while (true) {
    io_event evlist[IOWrapper::kEpollMaxEvents];
    // TODO ログ書き出し
    io_.writeLog();

    int ready = io_.wait_monitoring(evlist);
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
        if (listenSockets_.count(evlist[j].data.fd) > 0) {
          // 新しい接続を受け入れる
          acceptConnection(evlist[j].data.fd);
        }
        handleClientMessage(evlist[j].data.fd);
      } else if (evlist[j].events & EPOLLOUT) {
        // 書き込み可能になったソケットに対して処理を行う
        resendClientMessage(evlist[j].data.fd);
        // TODO ログ書き出し
        if (evlist[j].data.fd == IRCLogger::getInstance().getFd()) {
          io_.writeLog();
        }
      } else if (evlist[j].events & (EPOLLHUP | EPOLLERR)) {
        DEBUG_MSG("    closing fd " << evlist[j].data.fd);

        if (close(evlist[j].data.fd) == -1) {
          std::cerr << "close failed" << std::endl;
          std::exit(EXIT_FAILURE);
        }
        listenSockets_.erase(evlist[j].data.fd);
      }
    }
  }
}

const std::map<int, ClientSession*>& IRCServer::getClients() const {
  return clients_;
}

void IRCServer::addClient(ClientSession* client) {
  clients_[client->getFd()] = client;
  DEBUG_MSG("New client connected: " << client->getFd()
                                     << ", client num: " << clients_.size());
}
