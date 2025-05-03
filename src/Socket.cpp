#include "Socket.hpp"
#include <unistd.h>
#include <iostream>
#include "IRCLogger.hpp"

Socket::Socket(int fd) : fd_(fd) {
  if (fd_ < 0) {
    throw std::runtime_error("socket(2) failed");
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
