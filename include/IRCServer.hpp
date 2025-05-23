#pragma once
#ifndef __IRC_SERVER_HPP__
#define __IRC_SERVER_HPP__

#include <map>
#include <string>

#include "Channel.hpp"
#include "Client.hpp"
#include "IOWrapper.hpp"
#include "IRCMessage.hpp"
#include "IRCParser.hpp"
#include "Socket.hpp"

class IRCServer {
 private:
  // Member variables
  IOWrapper io_;
  std::string port_;
  std::string password_;
  std::map<int, Socket*> listenSockets_;  // ソケットFD→listeningソケット
  std::map<int, Client*> clients_;        // ソケットFD→クライアント
  std::map<std::string, Channel*>
      channels_;  // チャンネル名→チャンネルオブジェクト
  // UserManager userManager_;
  // ChannelManager channelManager_;
  // Logger logger_;
  void acceptConnection(int listenSocketFd);
  void sendResponses(const std::map<Client*, std::string>& res);
  void handleClientMessage(int clientFd);
  void resendClientMessage(int clientFd);
  void disconnectClient(Client* client);

  static const int BUFFER_SIZE = 1024;
  static const int MAX_MSG_SIZE = 510;  // IRCの仕様
  static const int MAX_BACKLOG = 100;

 public:
  // Orthodox Canonical Form
  IRCServer();
  IRCServer(const char* port, const char* password);
  ~IRCServer();
  IRCServer(const IRCServer& other);
  IRCServer& operator=(const IRCServer& other);

  // Getters
  const std::map<int, Client*>& getClients() const;
  const std::map<std::string, Channel*>& getChannels() const;
  Channel* getChannel(const std::string& name) const;
  const std::string& getPort() const;
  const std::string& getPassword() const;

  // Setters
  bool addClient(Client* client);
  bool addChannel(const std::string& name, Client* client);
  bool ifChannleExists(const std::string& name) const;
  // bool removeClient(Client* client);
  bool removeChannel(const std::string& name);

  // Member functions
  void startListen();  // ソケットをバインドしてリッスン状態にする
  void run();          // メインループ。接続受付、読み書き処理
  // void acceptConnection();
  // void receiveMessage(Client* client);
  // void sendMessage(Client* client, const std::string& message);
  // void disconnectClient(Client* client);
};

#endif  // __IRC_SERVER_HPP__
