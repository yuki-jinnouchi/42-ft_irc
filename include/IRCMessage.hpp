#pragma once
#ifndef __IRC_MESSAGE_HPP__
#define __IRC_MESSAGE_HPP__

#include <map>
#include <string>
#include <vector>

// #include "IRCParser.hpp"
#include "Client.hpp"

class IRCMessage {
 private:
  // Member variables
  Client* from_;
  std::string raw_;
  std::map<Client*, std::string> responses_;  // 宛先client->送信メッセージのmap
  std::string prefix_;
  std::string command_;
  std::vector<std::string> params_;

  // Constructor
  IRCMessage();

 public:
  // Orthodox Cannonical Form
  IRCMessage(Client* from, const std::string& raw);
  ~IRCMessage();
  IRCMessage(const IRCMessage& other);
  IRCMessage& operator=(const IRCMessage& other);

  // Getters
  Client* getFrom() const;
  const std::string& getRaw() const;
  const std::map<Client*, std::string>& getResponses() const;
  const std::string& getPrefix() const;
  const std::string& getCommand() const;
  // bool getIsReply() const;
  const std::string& getParam(int index) const;
  const std::vector<std::string>& getParams() const;

  // Setters
  void setRaw(const std::string& raw);
  void setPrefix(const std::string& prefix);
  void setCommand(const std::string& command);
  // void setIsReply(bool isReply);
  void addParam(const std::string& param);
  void setParams(const std::vector<std::string>& params);

  // Member functions
  // std::string toString() const;  // メッセージの文字列化
  bool isFromMe(const Client* client) const;
  void addResponse(Client* client, const std::string& response);
};

#endif  // __IRC_MESSAGE_HPP__
