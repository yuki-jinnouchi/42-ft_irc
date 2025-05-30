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
#include "RequestHandler.hpp"
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
  std::set<Client*> send_queue_;  // メッセージ送信待ちのクライアント
  RequestHandler request_handler_;

  // Logger logger_;
  void acceptConnection(int listenSocketFd);
  void sendResponses();
  void handleClientMessage(int clientFd);
  void resendClientMessage(int clientFd);
  void disconnectClient(Client* client);
  void startListen();  // ソケットをバインドしてリッスン状態にする

  static const int kMaxMsgSize = 510;  // IRCの仕様
  static const int kMaxBacklog = 100;

 public:
  // Orthodox Canonical Form
  IRCServer();
  IRCServer(const char* port, const char* password);
  ~IRCServer();
  // IRCServer(const IRCServer& other);
  // IRCServer& operator=(const IRCServer& other);

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
  void run();  // メインループ。接続受付、読み書き処理
  bool isNickTaken(const std::string nick) const;
  // void acceptConnection();
  // void receiveMessage(Client* client);
  // void sendMessage(Client* client, const std::string& message);
  // void disconnectClient(Client* client);
};

#endif  // __IRC_SERVER_HPP__
