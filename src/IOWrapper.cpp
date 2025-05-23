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
    ERROR_MSG("epoll_create failed");
    std::exit(EXIT_FAILURE);
  }
}

IOWrapper::~IOWrapper() {
  if (close(epfd_) == -1) {
    ERROR_MSG("close epfd failed.");
    std::exit(EXIT_FAILURE);
  }
}

bool IOWrapper::add_monitoring(int fd, uint32_t events) {
#ifndef UNIT_TEST
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
    ERROR_MSG("epoll_ctl add failed. fd: " << fd);
    return false;
  }
#endif
  return true;
}

bool IOWrapper::modify_monitoring(int fd, uint32_t events) {
#ifndef UNIT_TEST
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  if (epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1) {
    ERROR_MSG("epoll_ctl modify failed. fd: " << fd);
    return false;
  }
#endif
  return true;
}

bool IOWrapper::remove_monitoring(int fd) {
#ifndef UNIT_TEST
  if (epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, NULL) == -1) {
    ERROR_MSG("epoll_ctl remove failed. fd: " << fd);
    return false;
  }
#endif
  return true;
}

int IOWrapper::wait_monitoring(epoll_event* events) {
  return epoll_wait(epfd_, events, kEpollMaxEvents, -1);
}

bool IOWrapper::sendMessage(Client* client, const std::string& msg) {
  client->pushSendingMsg(msg);
  if (client->getSendingMsg().size() > Client::kMaxSendingMsgSize) {
    ERROR_MSG("Sending message size exceeds limit: "
              << client->getSendingMsg().size());
    return false;
  }
  return sendMessage(client);
}

bool IOWrapper::sendMessage(Client* client) {
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
        ERROR_MSG("send failed. fd: " << client->getFd());
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

bool IOWrapper::writeLog() {
  int log_fd = IRCLogger::getInstance().getFd();
  size_t log_size = IRCLogger::getInstance().getLog().size();
  if (log_size == 0) {
    return true;
  }
  while (log_size > 0) {
    ssize_t bytes_written =
        write(STDERR_FILENO, IRCLogger::getInstance().getLog().c_str(),
              IRCLogger::getInstance().getLog().size());
    if (bytes_written >= 0) {
      log_size = IRCLogger::getInstance().consumeLog(bytes_written);
      continue;
    } else {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // ノンブロッキングIOにより一時的に書き込みがブロックされているため、
        // EPOLLで書き込み可能になるまで監視
        modify_monitoring(log_fd, EPOLLIN | EPOLLOUT | EPOLLET);
        pending_write_fds_.insert(log_fd);
        return true;
      } else {
        ERROR_MSG("write failed");
        return false;
      }
    }
  }
  // すべてのログが出力された場合
  if (pending_write_fds_.find(log_fd) != pending_write_fds_.end()) {
    modify_monitoring(log_fd, EPOLLIN | EPOLLET);
    pending_write_fds_.erase(log_fd);
  }
  return true;
}

bool IOWrapper::setNonBlockingFlag(int fd) {
#ifndef UNIT_TEST
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    return false;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    return false;
  }
#endif
  return true;
}
