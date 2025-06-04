#include "Client.hpp"

#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "IRCLogger.hpp"

// Constructor & Destructor
Client::Client(int socketFd)
    : socket_(socketFd), nickName_(""), receiving_msg_(""), sending_msg_("") {}

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

// bool Client::getIsCapabilityNegotiating() const {
//   return isCapabilityNegotiating_;
// }

bool Client::getIsRegistered() const {
  return isRegistered_;
}

const std::string& Client::getReceivingMsg() const {
  return receiving_msg_;
}

std::string Client::getUserPrefix() const {
  return nickName_ + "!~" + userName_ + "@" + socket_.getHost();
  // :nick!~name@localhost
}

const std::map<std::string, Channel*>& Client::getJoinedChannels() const {
  return joinedChannels_;
}

bool Client::isJoinedChannel(const std::string& channelName) const {
  return joinedChannels_.find(channelName) != joinedChannels_.end();
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

void Client::setPassword(const std::string& password) {
  password_ = password;
}

// void Client::setIsCapabilityNegotiating(const bool isCapabilityNegotiating) {
//   isCapabilityNegotiating_ = isCapabilityNegotiating;
// }

void Client::addJoinedChannel(Channel* channel) {
  std::string channelName = channel->getName();
  joinedChannels_[channelName] = channel;
}

void Client::removeJoinedChannel(const std::string& channelName) {
  joinedChannels_.erase(channelName);
}

void Client::setIsRegistered(const bool isRegistered) {
  isRegistered_ = isRegistered;
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
