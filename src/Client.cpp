#include "Client.hpp"

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "IRCLogger.hpp"

// Constructor & Destructor
Client::Client(int socketFd)
    : socket_(socketFd), nickName_(""), receiving_msg_("") {}

Client::~Client() {}

// Getters
int Client::getFd() const {
  return socket_.getFd();
}

const std::string& Client::getNickName() const {
  return nickName_;
}

const std::string& Client::getUserName() const {
  return userName_;
}

const std::string& Client::getRealName() const {
  return realName_;
}

const std::string& Client::getPassword() const {
  return password_;
}

const std::string& Client::getReceivingMsg() const {
  return receiving_msg_;
}

// Setters
void Client::setNickName(const std::string& nick) {
  nickName_ = nick;
}

void Client::setUserName(const std::string& user) {
  userName_ = user;
}

void Client::setRealName(const std::string& real) {
  realName_ = real;
}

void Client::setPassword(const std::string& pass) {
  password_ = pass;
}

// Member Functions
std::string Client::popReceivingMsg() {
  std::string msg = receiving_msg_;
  receiving_msg_.clear();
  return msg;
}

const std::string& Client::getSendingMsg() {
  return sending_msg_;
}

size_t Client::consumeSendingMsg(size_t size) {
  if (size > sending_msg_.size()) {
    sending_msg_.clear();
  } else {
    sending_msg_.erase(0, size);
  }
  return sending_msg_.size();
}

void Client::pushSendingMsg(const std::string& msg) {
  sending_msg_ += msg;
}

void Client::pushReceivingMsg(const std::string& msg) {
  receiving_msg_ += msg;
}  

void Client::sendMessage(const std::string& msg) {
  if (send(socket_.getFd(), msg.c_str(), msg.size(), 0) == -1) {
    std::cerr << "send failed. fd: " << socket_.getFd()
              << ", nick: " << nickName_ << std::endl;
    throw std::runtime_error("send failed");
  }
}
