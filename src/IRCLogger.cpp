

#include "IRCLogger.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <iostream>

#include "IOWrapper.hpp"

#ifdef DEBUG
IRCLogger& IRCLogger::getInstance() {
  static IRCLogger instance;
  return instance;
}

IRCLogger::IRCLogger() {
  log_ = "";
  fd_ = open("/dev/stderr", O_WRONLY | O_NONBLOCK);
  if (fd_ < 0) {
    throw std::runtime_error("cannot open log file");
  }
}

IRCLogger::~IRCLogger() {
  std::cerr << log_;
  std::cerr.flush();
  close(fd_);
  log_.clear();
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
#endif  // DEBUG
