

#include "ClientSession.hpp"
#include <unistd.h>
#include <iostream>

ClientSession::ClientSession(int socketFd)
    : socketFd_(socketFd), nickName_("") {
  if (socketFd < 0) {
    throw std::runtime_error("accept(2) failed");
  }
  std::cout << "ClientSession created fd: " << socketFd_ << std::endl;
  socketFd_ = socketFd;
}

ClientSession::~ClientSession() {
  std::cout << "ClientSession close fd: " << socketFd_
            << ", nick: " << nickName_ << std::endl;
  close(socketFd_);
}

int ClientSession::getSocketFd() const {
  return socketFd_;
}

// ClientSession::ClientSession(const ClientSession& other) {
//   *this = other;
// }

// ClientSession& ClientSession::operator=(const ClientSession& other) {
//   if (this == &other) {
//     return *this;
//   }
//   socketFd_ = other.socketFd_;
//   nickName_ = other.nickName_;
//   return *this;
// }
