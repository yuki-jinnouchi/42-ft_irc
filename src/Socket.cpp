#include "Socket.hpp"

#include <unistd.h>

#include <iostream>

#include "IOWrapper.hpp"
#include "IRCLogger.hpp"

Socket::Socket(int fd) : fd_(fd) {
  // ソケットをノンブロッキングに設定
  if (!IOWrapper::setNonBlockingFlag(fd_)) {
    ERROR_MSG("fcntl: set non-blocking flag failed: fd" << fd_);
    close(fd_);
  }
  DEBUG_MSG("Socket created fd: " << fd_);
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
