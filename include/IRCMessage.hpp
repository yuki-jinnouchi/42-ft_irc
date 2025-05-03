#pragma once
#ifndef __IRC_MESSAGE_HPP__
#define __IRC_MESSAGE_HPP__

#include <map>
#include <string>
#include "ClientSession.hpp"

class IRCMessage {
 private:
  ClientSession* from_;
  std::string raw_;
  // 宛先client->送信メッセージのmap
  std::map<ClientSession*, std::string> responses_;
  // std::string prefix_;
  // std::string command_;
  // std::vector params_;
  IRCMessage();

 public:
  IRCMessage(ClientSession* from, const std::string& raw);
  ~IRCMessage();
  IRCMessage(const IRCMessage& other);
  IRCMessage& operator=(const IRCMessage& other);

  // bool parse(); // rawMessageを解析してprefix, command, paramsに分解
  // std::string toString() const;  // メッセージの文字列化

  ClientSession* const getFrom() const;
  const std::string& getRaw() const;
  const std::map<ClientSession*, std::string>& getResponses() const;

  // const std::string& getCommand() const;
  // const std::vector& getParams() const;
  // const std::string& getPrefix() const;
  bool isFromMe(const ClientSession* client) const;
  void addResponse(ClientSession* client, const std::string& response);
};

#endif  // __IRC_MESSAGE_HPP__
