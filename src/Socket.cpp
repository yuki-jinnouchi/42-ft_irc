#include "Socket.hpp"
#include <unistd.h>
#include <iostream>
#include "IOWrapper.hpp"
#include "IRCLogger.hpp"

Socket::Socket(int fd) : fd_(fd) {
  if (fd_ < 0) {
    throw std::runtime_error("socket(2) failed");
  }
  // ソケットをノンブロッキングに設定
  if (!IOWrapper::setNonBlockingFlag(fd_)) {
    std::cerr << "fcntl: set non-blocking flag failed: fd" << fd_ << std::endl;
    close(fd_);
    throw std::runtime_error("fcntl: set non-blocking flag failed");
  }
  DEBUG_MSG("Socket created fd: " << fd_);
}

Socket::~Socket() {
  if (fd_ != -1) {
    if (close(fd_) == -1) {
      std::cerr << "close failed" << std::endl;
    }
    DEBUG_MSG("Socket closed fd: " << fd_);
  }
}

int Socket::getFd() const {
  return fd_;
}
