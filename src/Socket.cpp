#include "Socket.hpp"
#include <unistd.h>
#include <iostream>

Socket::Socket(int fd) : fd_(fd) {
  if (fd_ < 0) {
    throw std::runtime_error("socket(2) failed");
  }
  std::cout << "Socket created fd: " << fd_ << std::endl;
}

Socket::~Socket() {
  if (fd_ != -1) {
    if (close(fd_) == -1) {
      std::cerr << "close failed" << std::endl;
    }
    std::cout << "Socket closed fd: " << fd_ << std::endl;
  }
}

int Socket::getFd() const {
  return fd_;
}
