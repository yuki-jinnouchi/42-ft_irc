

#include "IRCLogger.hpp"

#include <unistd.h>

#include <iostream>

#include "IOWrapper.hpp"

IRCLogger& IRCLogger::getInstance() {
  static IRCLogger instance;
  return instance;
}

IRCLogger::IRCLogger() {
  log_ = "";
  fd_ = STDERR_FILENO;
  if (!IOWrapper::setNonBlockingFlag(fd_)) {
    throw std::runtime_error("fcntl: set non-blocking flag failed");
  }
}

IRCLogger::~IRCLogger() {
  std::cerr << log_;
  std::cerr.flush();
}

const std::string& IRCLogger::getLog() const {
  return log_;
}

int IRCLogger::getFd() const {
  return fd_;
}

size_t IRCLogger::consumeLog(size_t size) {
  if (size > log_.size()) {
    log_.clear();
    return 0;
  }
  log_.erase(0, size);
  return log_.size();
}
