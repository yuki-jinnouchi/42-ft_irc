#pragma once
#ifndef __IRC_SERVER_HPP__
#define __IRC_SERVER_HPP__

#include <map>
#include <string>

#include "ClientSession.hpp"
#include "IOWrapper.hpp"
#include "IRCMessage.hpp"
#include "IRCParser.hpp"
#include "Socket.hpp"

class IRCServer {
 private:
  IOWrapper io_;
  std::string port_;
  std::string password_;
  std::map<int, Socket*> listenSockets_;   // ソケットFD→listeningソケット
  std::map<int, ClientSession*> clients_;  // ソケットFD→クライアント
  // UserManager userManager_;
  // ChannelManager channelManager_;
  // Logger logger_;
  void acceptConnection(int listenSocketFd);
  void sendResponses(const IRCMessage& msg);
  void handleClientMessage(int clientFd);
  void resendClientMessage(int clientFd);
  void disconnectClient(ClientSession* client);

  static const int BUFFER_SIZE = 1024;
  static const int MAX_MSG_SIZE = 510;  // IRCの仕様

  static const int MAX_BACKLOG = 100;

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

  // clients_を取得
  const std::map<int, ClientSession*>& getClients() const;
  void addClient(ClientSession* client);
};

#endif  // __IRC_SERVER_HPP__
