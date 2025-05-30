#pragma once
#ifndef __ACOMMAND_HPP__
#define __ACOMMAND_HPP__

#include <string>
#include <vector>

#include "IRCServer.hpp"
#include "IRCMessage.hpp"

enum ResponseCode {
  // Connection Registration
  RPL_WELCOME = 1,
  RPL_YOURHOST = 2,
  RPL_CREATED = 3,
  RPL_MYINFO = 4,

  // Channel Operations
  RPL_TOPIC = 332,
  RPL_NAMREPLY = 353,
  RPL_ENDOFNAMES = 366,

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

class ACommand {
 protected:
  // Member variables
  IRCServer* server_;
  std::string commandName_;

  // Member functions
  // ResponserBuilder
  void addResponseText(Client* client, const std::string& text);
  void addResponseByCode(std::vector<Client*> Clients, ResponseCode responseCode);
  std::string formatResponse(int const responseCode, std::string const& fromat,
                             const std::vector<std::string>& values);
  std::string generateResponse(int responseCode);

  // Registration
  void registrate(IRCMessage& msg);

 public:
  // Orthodox Canonical Form
  // ACommand();
  ACommand(IRCServer* server, std::string commandName);
  virtual ~ACommand();
  // virtual ACommand(const ACommand& other);
  // virtual ACommand& operator=(const ACommand& other);

  // Getters
  const std::string& getCommandName() const;

  // Member functions
  virtual void execute(IRCMessage& msg) = 0;
};

#endif  // __ACOMMAND_HPP__
