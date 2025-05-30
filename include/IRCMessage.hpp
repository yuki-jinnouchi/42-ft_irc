#pragma once
#ifndef __IRC_MESSAGE_HPP__
#define __IRC_MESSAGE_HPP__

#include <map>
#include <string>
#include <vector>

#include "Client.hpp"

// https://www.rfc-editor.org/rfc/rfc1459#section-6
enum IRCRplCode {
  RPL_NONE = 300,

  // Connection Registration
  RPL_WELCOME = 1,
  RPL_YOURHOST = 2,
  RPL_CREATED = 3,
  RPL_MYINFO = 4,

  // Channel Operations
  RPL_TOPIC = 332,
  RPL_NAMREPLY = 353,
  RPL_ENDOFNAMES = 366
};

enum IRCErrCode {
  ERR_NONE = 0,

  // Error Codes
  ERR_NOSUCHNICK = 401,
  ERR_NOSUCHCHANNEL = 403,
  ERR_CANNOTSENDTOCHAN = 404,
  ERR_TOOMANYCHANNELS = 405,
  ERR_UNKNOWNCOMMAND = 421,
  ERR_NONICKNAMEGIVEN = 431,
  ERR_ERRONEUSNICKNAME = 432,
  ERR_NICKNAMEINUSE = 433,
  ERR_USERNOTINCHANNEL = 441,
  ERR_NOTONCHANNEL = 442,
  ERR_USERONCHANNEL = 443,
  ERR_NOTREGISTERED = 451,
  ERR_NEEDMOREPARAMS = 461,
  ERR_ALREADYREGISTRED = 462,
  ERR_PASSWDMISMATCH = 464,
  ERR_CHANNELISFULL = 471,
  ERR_UNKNOWNMODE = 472,
  ERR_INVITEONLYCHAN = 473,
  ERR_BADCHANNELKEY = 475,
  ERR_CHANOPRIVSNEEDED = 482
};

class IRCMessage {
 private:
  // Member variables
  Client* from_;
  Client* to_;
  std::string raw_;
  std::map<Client*, std::string> responses_;  // 宛先client->送信メッセージのmap
  std::string prefix_;
  std::string command_;

  IRCRplCode rpl_code_;  // レスポンスコード
  IRCErrCode err_code_;  // エラーコード
  std::string error_msg_;

  std::vector<std::string> params_;

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
  IRCRplCode getRplCode() const;
  IRCErrCode getErrCode() const;

  // Setters
  void setFrom(Client* from);
  void setTo(Client* to);
  void setRaw(const std::string& raw);
  void setPrefix(const std::string& prefix);
  void setCommand(const std::string& command);
  // void setIsReply(bool isReply);
  void addParam(const std::string& param);
  void setParams(const std::vector<std::string>& params);
  void setRplCode(IRCRplCode rpl_code);
  void setErrCode(IRCErrCode err_code);

  // Member functions
  // std::string toString() const;  // メッセージの文字列化
  bool isFromMe(const Client* client) const;
  void addResponse(Client* client, const std::string& response);
};

#endif  // __IRC_MESSAGE_HPP__
