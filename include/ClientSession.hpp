#pragma once
#ifndef __CLIENT_SESSION_HPP__
#define __CLIENT_SESSION_HPP__

#include <string>

class ClientSession {
 private:
  int socketFd_;
  std::string nickName_;
  // std::string userName_;
  // std::string realName_;
  // bool registered_; // NICKとUSER登録済みか
  // std::set joinedChannels_;

  ClientSession();
  ClientSession(const ClientSession& other);
  ClientSession& operator=(const ClientSession& other);

 public:
  ClientSession(int socketFd);
  ~ClientSession();

  int getSocketFd() const;
  const std::string& getNickName() const;
  // void setNickName(const std::string& nick);
  // void setUserInfo(const std::string& user, const std::string& real);
  // bool isRegistered() const;

  // void joinChannel(Channel* channel);
  // void partChannel(Channel* channel);
  // const std::set& getChannels() const;
};

#endif  // __CLIENT_SESSION_HPP__
