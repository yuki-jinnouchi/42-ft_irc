#include "IRCServer.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "ClientSession.hpp"

#define MAX_BACKLOG 5

IRCServer::IRCServer(const char* port, const char* password) {
  // TODO: ポート番号のバリデーション
  std::istringstream ss(port);
  ss >> port_;

  password_ = std::string(password);
  std::cout << "Port: " << port_ << ", Password: " << password_ << std::endl;
}

IRCServer::~IRCServer() {
  // ソケットを閉じる
  for (size_t i = 0; i < listenSocketFds_.size(); ++i) {
    std::cout << "Shutdown IRC Server: socket fd: " << listenSocketFds_[i]
              << std::endl;
    close(listenSocketFds_[i]);
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
    listenSocketFds_.push_back(sockfd);
  }
  freeaddrinfo(res);  // アドレス情報の解放
  if (listenSocketFds_.empty()) {
    std::cerr << "Error: socket/bind/listen failed" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void IRCServer::run() {
  sockaddr_storage addr;
  socklen_t addrlen = sizeof(addr);
  std::cout << "------1" << std::endl;
  int sockfd = accept(listenSocketFds_[0], (struct sockaddr*)&addr, &addrlen);
  std::cout << "------2" << std::endl;
  ClientSession client(sockfd);
  // 日付を返す
  time_t t;
  time(&t);
  char* timestr = ctime(&t);
  // write(sockfd, timestr, strlen(timestr));
  send(client.getSocketFd(), timestr, strlen(timestr), 0);
}

// void IRCServer::acceptConnection() {}
