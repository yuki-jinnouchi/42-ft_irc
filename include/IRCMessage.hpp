#pragma once
#ifndef __IRC_MESSAGE_HPP__
#define __IRC_MESSAGE_HPP__

#include <map>
#include <string>
#include <vector>

// #include "IRCParser.hpp"
#include "Client.hpp"

// https://www.rfc-editor.org/rfc/rfc1459.html
typedef enum {
  ERR_NONE = 0,                // No error
  ERR_NOSUCHNICK = 401,        // No such nick/channel
  ERR_NOSUCHSERVER = 402,      // No such server
  ERR_NOSUCHCHANNEL = 403,     // No such channel
  ERR_CANNOTSENDTOCHAN = 404,  // Cannot send to channel
  ERR_TOOMANYCHANNELS = 405,   // Too many channels
  // ...
  ERR_NOORIGIN = 409,  // No origin specified
  // etc...
} IRCErrorCode;

typedef enum {
  RPL_NONE = 300,  // Dummy reply code
  RPL_USERHOST = 302,
  RPL_ISON = 303,
  RPL_AWAY = 301,     // Away
  RPL_UNAWAY = 305,   // You are no longer marked as being away
  RPL_NOWAWAY = 306,  // You have been marked as being away
  // etc...
} IRCRplCode;

class IRCMessage {
 private:
  // Member variables
  Client* from_;
  Client* to_;
  IRCErrorCode error_code_;  // エラーコード
  IRCRplCode rpl_code_;      // RPLコード
  std::string raw_;
  std::map<Client*, std::string> responses_;  // 宛先client->送信メッセージのmap
  std::string prefix_;
  std::string command_;
  std::vector<std::string> params_;
  std::string error_msg_;  // エラーメッセージ

 public:
  // Orthodox Cannonical Form
  IRCMessage();
  IRCMessage(Client* from, const std::string& raw);
  ~IRCMessage();
  IRCMessage(const IRCMessage& other);
  IRCMessage& operator=(const IRCMessage& other);

  // Getters
  Client* getFrom() const;
  Client* getTo() const;
  const std::string& getRaw() const;
  const std::map<Client*, std::string>& getResponses() const;
  const std::string& getPrefix() const;
  const std::string& getCommand() const;
  // bool getIsReply() const;
  const std::string& getParam(int index) const;
  const std::vector<std::string>& getParams() const;
  IRCErrorCode getErrorCode() const;
  IRCRplCode getRplCode() const;
  const std::string& getErrorMsg() const;

  // Setters
  void setTo(Client* to);
  void setRaw(const std::string& raw);
  void setPrefix(const std::string& prefix);
  void setCommand(const std::string& command);
  // void setIsReply(bool isReply);
  void addParam(const std::string& param);
  void setParams(const std::vector<std::string>& params);
  void setErrorCode(IRCErrorCode error_code);
  void setRplCode(IRCRplCode rpl_code);
  void setErrorMsg(const std::string& error_msg);

  // Member functions
  // std::string toString() const;  // メッセージの文字列化
  bool isFromMe(const Client* client) const;
  void pushResponse(Client* client, const std::string& response);
};

#endif  // __IRC_MESSAGE_HPP__
