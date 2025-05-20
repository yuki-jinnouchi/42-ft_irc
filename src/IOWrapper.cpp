#include "IOWrapper.hpp"
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "IRCLogger.hpp"

IOWrapper::IOWrapper() {
  epfd_ = epoll_create1(EPOLL_CLOEXEC);
  if (epfd_ == -1) {
    std::cerr << "epoll_create failed\n";
    std::exit(EXIT_FAILURE);
  }
}

IOWrapper::~IOWrapper() {
  if (close(epfd_) == -1) {
    std::cerr << "close failed" << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

bool IOWrapper::add_monitoring(int fd, uint32_t events) {
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
    std::cerr << "epoll_ctl failed" << std::endl;
    return false;
  }
  return true;
}

bool IOWrapper::modify_monitoring(int fd, uint32_t events) {
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
    std::cerr << "epoll_ctl failed" << std::endl;
    return false;
  }
  return true;
}

bool IOWrapper::remove_monitoring(int fd) {
  if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) == -1) {
    std::cerr << "epoll_ctl failed" << std::endl;
    return false;
  }
  return true;
}

int IOWrapper::wait_monitoring(epoll_event* events) {
  return epoll_wait(epfd_, events, kEpollMaxEvents, -1);
}

bool IOWrapper::sendMessage(ClientSession* client, const std::string& msg) {
  client->pushSendingMsg(msg);
  if (client->getSendingMsg().size() > ClientSession::kMaxSendingMsgSize) {
    std::cerr << "Sending message size exceeds limit: "
              << client->getSendingMsg().size() << std::endl;
    return false;
  }
  return sendMessage(client);
}

bool IOWrapper::sendMessage(ClientSession* client) {
  size_t msg_size = client->getSendingMsg().size();
  if (msg_size == 0) {
    // 送信するメッセージがない場合は何もしない
    return true;
  }
  while (msg_size > 0) {
    ssize_t bytes_sent = send(client->getFd(), client->getSendingMsg().c_str(),
                              client->getSendingMsg().size(), 0);
    if (bytes_sent >= 0) {
      // 送信したバイト数を送信待ちメッセージから削除
      msg_size = client->consumeSendingMsg(bytes_sent);
      continue;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // ノンブロッキングIOにより一時的に書き込みがブロックされているため、
        // EPOLLで書き込み可能になるまで監視
        modify_monitoring(client->getFd(), EPOLLIN | EPOLLOUT | EPOLLET);
        pending_write_fds_.insert(client->getFd());
        return true;
      } else {
        std::cerr << "send failed. fd: " << client->getFd() << std::endl;
        return false;
      }
    }
  }
  // すべてのメッセージが送信された場合
  if (pending_write_fds_.find(client->getFd()) != pending_write_fds_.end()) {
    modify_monitoring(client->getFd(), EPOLLIN | EPOLLET);
    pending_write_fds_.erase(client->getFd());
  }
  return true;
}

bool IOWrapper::setNonBlockingFlag(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return false;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    return false;
  }
  return true;
}
