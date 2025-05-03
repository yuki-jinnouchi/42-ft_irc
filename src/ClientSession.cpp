#include "ClientSession.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

ClientSession::ClientSession(int socketFd) : socket_(socketFd), nickName_("") {}

ClientSession::~ClientSession() {}

void ClientSession::sendMessage(const std::string& msg) {
  if (send(socket_.getFd(), msg.c_str(), msg.size(), 0) == -1) {
    std::cerr << "send failed. fd: " << socket_.getFd()
              << ", nick: " << nickName_ << std::endl;
    throw std::runtime_error("send failed");
  }
}

int ClientSession::getFd() const {
  return socket_.getFd();
}
