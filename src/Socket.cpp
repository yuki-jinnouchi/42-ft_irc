#include "Socket.hpp"

#include <unistd.h>

#include <iostream>

#include "IOWrapper.hpp"
#include "IRCLogger.hpp"

Socket::Socket(int fd) : fd_(fd) {
  DEBUG_MSG("Socket created fd: " << fd_);
  // TODO: ホストを取得
  host_ = "localhost";
}

Socket::~Socket() {
  if (fd_ != -1) {
    if (close(fd_) == -1) {
      ERROR_MSG("close failed");
    }
    DEBUG_MSG("Socket closed fd: " << fd_);
  }
}

int Socket::getFd() const {
  return fd_;
}

const std::string& Socket::getHost() const {
  return host_;
}
