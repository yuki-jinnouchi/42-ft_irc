#pragma once
#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include <string>

class Socket {
 private:
  int fd_;
  std::string host_;

  Socket();
  Socket(const Socket& other);
  Socket& operator=(const Socket& other);

 public:
  Socket(int socketFd);
  ~Socket();

  int getFd() const;
  const std::string& getHost() const;
};

#endif  // __SOCKET_HPP__
