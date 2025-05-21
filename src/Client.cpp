#include "Client.hpp"

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "IRCLogger.hpp"

ClientSession::ClientSession(int socketFd)
    : socket_(socketFd), nickName_(""), receiving_msg_("") {}

ClientSession::~ClientSession() {}

int ClientSession::getFd() const { return socket_.getFd(); }

const std::string& ClientSession::getNickName() const { return nickName_; }

void ClientSession::setNickName(const std::string& nick) { nickName_ = nick; }

const std::string& ClientSession::getReceivingMsg() { return receiving_msg_; }

std::string ClientSession::popReceivingMsg() {
  std::string msg = receiving_msg_;
  receiving_msg_.clear();
  return msg;
}

void ClientSession::pushReceivingMsg(const std::string& msg) {
  receiving_msg_ += msg;
}

const std::string& ClientSession::getSendingMsg() { return sending_msg_; }

size_t ClientSession::consumeSendingMsg(size_t size) {
  if (size > sending_msg_.size()) {
    sending_msg_.clear();
  } else {
    sending_msg_.erase(0, size);
  }
  return sending_msg_.size();
}

void ClientSession::pushSendingMsg(const std::string& msg) {
  sending_msg_ += msg;
}
