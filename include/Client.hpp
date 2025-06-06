#pragma once
#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <map>
#include <string>
#include <vector>

#include "Channel.hpp"
#include "Socket.hpp"

class Channel;

class Client {
 private:
  // Member variables
  Socket socket_;
  std::string nickName_;
  std::string userName_;
  std::string realName_;
  std::string password_;  // パスワード
  std::map<std::string, Channel*> joinedChannels_;
  std::string receiving_msg_;  // 受信途中のメッセージ
  std::string sending_msg_;    // 送信途中のメッセージ
  // bool isCapabilityNegotiating_;  // CAPABILITYネゴシエーション中か
  bool isRegistered_;  // NICKとUSER登録済みか

  // Constractor & Destructor
  Client();
  // // Clientをdeleteした時にソケットをクローズするため、コピーは許可しない
  Client(const Client& other);
  Client& operator=(const Client& other);

 public:
  static const int kMaxSendingMsgSize = 512 * 100;  // 送信バッファサイズ
  static const int kMaxNickNameSize = 9;            // NICK名の最大長

  // Constructor & Destructor
  ~Client();
  Client(int socketFd);

  // Getters
  int getFd() const;
  const std::string& getNickName() const;
  const std::string& getUserName() const;
  const std::string& getRealName() const;
  const std::string& getPassword() const;
  const std::string& getReceivingMsg() const;
  // bool getIsCapabilityNegotiating() const;
  bool getIsRegistered() const;
  std::string getUserPrefix() const;
  const std::map<std::string, Channel*>& getJoinedChannels() const;
  bool isJoinedChannel(const std::string& channelName) const;

  // Setters
  void setNickName(const std::string& name);
  void setUserName(const std::string& name);
  void setRealName(const std::string& name);
  void setPassword(const std::string& password);
  // void setIsCapabilityNegotiating(const bool isCapabilityNegotiating);
  void setIsRegistered(const bool isRegisterd);
  void addJoinedChannel(Channel* channel);
  void removeJoinedChannel(const std::string& channelName);

  // Member functions
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

#endif  // __CLIENT_HPP__
