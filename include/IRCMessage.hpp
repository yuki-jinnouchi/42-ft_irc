#pragma once
#ifndef __IRC_MESSAGE_HPP__
#define __IRC_MESSAGE_HPP__

#include <map>
#include <string>
#include <vector>

#include "IRCParser.hpp"
#include "ClientSession.hpp"

class IRCMessage {
 private:
  // Member variables
  ClientSession* from_;
  std::string raw_;
  std::map<ClientSession*, std::string> responses_; // 宛先client->送信メッセージのmap
  std::string prefix_;
  std::string command_;
  bool isReply_;
  std::vector<std::string> params_;

  // Constructor
  IRCMessage();

 public:
  // Orthodox Cannonical Form
  IRCMessage(ClientSession* from, const std::string& raw);
  ~IRCMessage();
  IRCMessage(const IRCMessage& other);
  IRCMessage& operator=(const IRCMessage& other);

  // Getters
  ClientSession* getFrom() const;
  const std::string& getRaw() const;
  const std::map<ClientSession*, std::string>& getResponses() const;
  const std::string& getPrefix() const;
  const std::string& getCommand() const;
  bool isReply() const;
  const std::string& getParam(int index) const;
  const std::vector<std::string>& getParams() const;

  // Setters
  void setRaw(const std::string& raw);
  void setPrefix(const std::string& prefix);
  void setCommand(const std::string& command);
  void setReply(bool isReply);
  void addParam(const std::string& param);
  void setParams(const std::vector<std::string>& params);

  // Member functions
  // std::string toString() const;  // メッセージの文字列化
  bool isFromMe(const ClientSession* client) const;
  void addResponse(ClientSession* client, const std::string& response);
};

#endif  // __IRC_MESSAGE_HPP__
