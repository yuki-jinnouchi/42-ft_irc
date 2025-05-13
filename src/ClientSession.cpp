#include "ClientSession.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

ClientSession::ClientSession(int socketFd)
    : socket_(socketFd), nickName_(""), receiving_msg_("") {}

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

const std::string& ClientSession::getNickName() const {
  return nickName_;
}

void ClientSession::setNickName(const std::string& nick) {
  nickName_ = nick;
}

const std::string& ClientSession::getReceivingMsg() {
  return receiving_msg_;
}

std::string ClientSession::popReceivingMsg() {
  std::string msg = receiving_msg_;
  receiving_msg_.clear();
  return msg;
}

void ClientSession::pushReceivingMsg(const std::string& msg) {
  receiving_msg_ += msg;
}
