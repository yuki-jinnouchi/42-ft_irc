#include "Client.hpp"

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "IRCLogger.hpp"

Client::Client(int socketFd)
    : socket_(socketFd), nickName_(""), receiving_msg_("") {}

Client::~Client() {}

int Client::getFd() const { return socket_.getFd(); }

const std::string& Client::getNickName() const { return nickName_; }

void Client::setNickName(const std::string& nick) { nickName_ = nick; }

const std::string& Client::getReceivingMsg() { return receiving_msg_; }

std::string Client::popReceivingMsg() {
  std::string msg = receiving_msg_;
  receiving_msg_.clear();
  return msg;
}

void Client::pushReceivingMsg(const std::string& msg) { receiving_msg_ += msg; }

const std::string& Client::getSendingMsg() { return sending_msg_; }

size_t Client::consumeSendingMsg(size_t size) {
  if (size > sending_msg_.size()) {
    sending_msg_.clear();
  } else {
    sending_msg_.erase(0, size);
  }
  return sending_msg_.size();
}

void Client::pushSendingMsg(const std::string& msg) { sending_msg_ += msg; }
