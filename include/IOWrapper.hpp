#pragma once
#ifndef __I_O_WRAPPER_HPP__
#define __I_O_WRAPPER_HPP__

#include <stdint.h>

#include <set>

#include "Client.hpp"

typedef struct epoll_event io_event;

class IOWrapper {
 public:
  static const int kEpollMaxEvents = 10000;
  static const int kRecvBufferSize = 1024;

  IOWrapper();
  ~IOWrapper();

  bool add_monitoring(int fd, uint32_t events);
  bool modify_monitoring(int fd, uint32_t events);
  bool remove_monitoring(int fd);
  int wait_monitoring(io_event* events);

  bool sendMessage(Client* client);

  bool receiveMessage(Client* client, std::string& msg);

  bool writeLog(int fd);

 private:
  int epfd_;                         // epollのファイルディスクリプタ
  std::set<int> pending_write_fds_;  // 書き込み待ちFDのセット

  IOWrapper(const IOWrapper& other);
  IOWrapper& operator=(const IOWrapper& other);
};

#endif  // __I_O_WRAPPER_HPP__
