#pragma once
#ifndef __CLIENT_SESSION_HPP__
#define __CLIENT_SESSION_HPP__

#include <string>

#include "Socket.hpp"

class Client {
 private:
  Socket socket_;
  std::string nickName_;
  // std::string userName_;
  // std::string realName_;
  // bool registered_; // NICKとUSER登録済みか
  // std::set joinedChannels_;
  std::string receiving_msg_;  // 受信途中のメッセージ
  std::string sending_msg_;    // 送信途中のメッセージ

  Client();
  // Clientをdeleteした時にソケットをクローズするため、コピーは許可しない
  Client(const Client& other);
  Client& operator=(const Client& other);

 public:
  static const int kMaxSendingMsgSize = 512 * 100;  // 送信バッファサイズ

  Client(int socketFd);
  ~Client();

  int getFd() const;
  const std::string& getNickName() const;
  void setNickName(const std::string& nick);
  const std::string& getReceivingMsg();
  std::string popReceivingMsg();
  void pushReceivingMsg(const std::string& msg);
  const std::string& getSendingMsg();
  size_t consumeSendingMsg(size_t size);
  void pushSendingMsg(const std::string& msg);
  // void setUserInfo(const std::string& user, const std::string& real);
  // bool isRegistered() const;

  // void joinChannel(Channel* channel);
  // void partChannel(Channel* channel);
  // const std::set& getChannels() const;
};

#endif  // __CLIENT_SESSION_HPP__
