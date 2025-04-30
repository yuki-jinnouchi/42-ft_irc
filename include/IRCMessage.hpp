#pragma once
#ifndef __IRC_MESSAGE_HPP__
#define __IRC_MESSAGE_HPP__

#include <string>
#include "ClientSession.hpp"

class IRCMessage {
 private:
  ClientSession* from_;
  std::string raw_;
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

  const ClientSession* getFrom() const;
  const std::string& getRaw() const;
  // const std::string& getCommand() const;
  // const std::vector& getParams() const;
  // const std::string& getPrefix() const;
};

#endif  // __IRC_MESSAGE_HPP__
