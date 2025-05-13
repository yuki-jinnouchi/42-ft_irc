#pragma once
#ifndef __CLIENT_SESSION_HPP__
#define __CLIENT_SESSION_HPP__

#include <string>
#include "Socket.hpp"

class ClientSession {
 private:
  Socket socket_;
  std::string nickName_;
  // std::string userName_;
  // std::string realName_;
  // bool registered_; // NICKとUSER登録済みか
  // std::set joinedChannels_;
  std::string receiving_msg_;  // 受信途中のメッセージ

  ClientSession();
  // ClientSessionをdeleteした時にソケットをクローズするため、コピーは許可しない
  ClientSession(const ClientSession& other);
  ClientSession& operator=(const ClientSession& other);

 public:
  ClientSession(int socketFd);
  ~ClientSession();

  int getFd() const;
  const std::string& getNickName() const;
  void setNickName(const std::string& nick);
  const std::string& getReceivingMsg();
  std::string popReceivingMsg();
  void pushReceivingMsg(const std::string& msg);
  // void setUserInfo(const std::string& user, const std::string& real);
  // bool isRegistered() const;

  // void joinChannel(Channel* channel);
  // void partChannel(Channel* channel);
  // const std::set& getChannels() const;
  void sendMessage(const std::string& message);
};

#endif  // __CLIENT_SESSION_HPP__
