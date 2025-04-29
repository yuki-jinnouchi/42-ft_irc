#pragma once
#ifndef __IRC_SERVER_HPP__
#define __IRC_SERVER_HPP__

#include <string>
#include <vector>
// #include "ClientSession.hpp"

class IRCServer {
 private:
  std::string port_;
  std::string password_;
  std::vector<int> listenSocketFds_;
  // std::map<int, ClientSession*> clients_; // ソケットFD→クライアント
  // UserManager userManager_;
  // ChannelManager channelManager_;
  // Logger logger_;

 public:
  IRCServer(const char* port, const char* password);
  ~IRCServer();
  IRCServer(const IRCServer& other);
  IRCServer& operator=(const IRCServer& other);

  void startListen();  // ソケットをバインドしてリッスン状態にする
  void run();          // メインループ。接続受付、読み書き処理
  // void acceptConnection();
  // void receiveMessage(ClientSession* client);
  // void sendMessage(ClientSession* client, const std::string& message);
  // void disconnectClient(ClientSession* client);
};

#endif  // __IRC_SERVER_HPP__
