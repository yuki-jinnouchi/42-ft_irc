#pragma once
#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

class Socket {
 private:
  int fd_;

  Socket();
  Socket(const Socket& other);
  Socket& operator=(const Socket& other);

 public:
  Socket(int socketFd);
  ~Socket();

  int getFd() const;
};

#endif  // __SOCKET_HPP__
