#pragma once
#ifndef __IRC_LOGGER_HPP__
#define __IRC_LOGGER_HPP__

#include <iostream>
#include <sstream>

#ifdef DEBUG
#define DEBUG_MSG(msg)                                         \
  do {                                                         \
    IRCLogger::getInstance()                                   \
        << "\033[30mDEBUG: " << msg << "\033[0m" << std::endl; \
  } while (0)
#else
#define DEBUG_MSG(msg)
#endif

#define ERROR_MSG(msg)                                                        \
  do {                                                                        \
    IRCLogger::getInstance() << "\033[31mERROR\033[0m: " << msg << std::endl; \
  } while (0)

#define INFO_MSG(msg)                                                        \
  do {                                                                       \
    IRCLogger::getInstance() << "\033[36mINFO\033[0m: " << msg << std::endl; \
  } while (0)

class IRCLogger {
 public:
  // シングルトン
  static IRCLogger& getInstance();

  const std::string& getLog() const;
  int getFd() const;
  size_t consumeLog(size_t size);

  template <typename T>
  IRCLogger& operator<<(const T& value) {
    std::ostringstream oss;
    oss << value;
    log_ += oss.str();
    return *this;
  }

  IRCLogger& operator<<(std::ostream& (*manip)(std::ostream&)) {
    std::ostringstream oss;
    oss << manip;
    log_ += oss.str();

    return *this;
  }

 private:
  std::string log_;
  int fd_;

  IRCLogger();
  ~IRCLogger();
  IRCLogger(const IRCLogger& other);
  IRCLogger& operator=(const IRCLogger& other);
};

#endif  // __IRC_LOGGER_HPP__
