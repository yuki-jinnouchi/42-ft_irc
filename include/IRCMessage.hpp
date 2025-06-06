#pragma once
#ifndef __IRC_MESSAGE_HPP__
#define __IRC_MESSAGE_HPP__

#include <map>
#include <string>
#include <vector>

// #include "IRCParser.hpp"
#include "Client.hpp"
#include "ResponseCode.hpp"

class IRCMessage {
 private:
  // Member variables
  Client* from_;
  Client* to_;
  std::string raw_;
  std::string prefix_;
  std::string command_;

  IRCResponseCode res_code_;
  std::string body_;
  std::vector<std::string> params_;

  // Constructor
  IRCMessage();

 public:
  // Orthodox Cannonical Form
  IRCMessage(Client* from, const std::string& raw);
  IRCMessage(Client* from, Client* to);
  ~IRCMessage();
  IRCMessage(const IRCMessage& other);
  IRCMessage& operator=(const IRCMessage& other);

  // Getters
  Client* getFrom() const;
  Client* getTo() const;
  const std::string& getRaw() const;
  const std::string& getPrefix() const;
  const std::string& getCommand() const;
  const std::string& getParam(int index) const;
  const std::vector<std::string>& getParams() const;
  IRCResponseCode getResCode() const;
  std::string getBody() const;

  // Setters
  void setFrom(Client* from);
  void setTo(Client* to);
  void setRaw(const std::string& raw);
  void setPrefix(const std::string& prefix);
  void setCommand(const std::string& command);
  void addParam(const std::string& param);
  void setParams(const std::vector<std::string>& params);
  void setResCode(IRCResponseCode res_code);
  void setBody(const std::string& body);

  // Member functions
  // std::string toString() const;  // メッセージの文字列化
  bool isFromMe(const Client* client) const;
};

#endif  // __IRC_MESSAGE_HPP__
