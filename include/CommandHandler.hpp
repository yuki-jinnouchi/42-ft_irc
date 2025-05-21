#pragma once
#ifndef __COMMAND_HANDLER_HPP__
#define __COMMAND_HANDLER_HPP__

#include <string>

#include "IRCMessage.hpp"
#include "IRCServer.hpp"

class CommandHandler {
 private:
  IRCServer* server_;

 private:
  // void handleUser(Client* client, const IRCMessage& message);
  // void handleJoin(Client* client, const IRCMessage& message);
  // void handlePrivmsg(Client* client, const IRCMessage& message);
  // void handlePart(Client* client, const IRCMessage& message);
  // void handleQuit(Client* client, const IRCMessage& message);
  // void handlePong(Client* client, const IRCMessage& message);

 public:
  CommandHandler(IRCServer* server);
  ~CommandHandler();
  CommandHandler(const CommandHandler& other);
  CommandHandler& operator=(const CommandHandler& other);

  const std::map<Client*, std::string>& handleCommand(IRCMessage& msg);
  const std::map<Client*, std::string>& broadCastRawMsg(IRCMessage& msg);
  const std::map<Client*, std::string>& handleNick(IRCMessage& msg);
  const std::map<Client*, std::string>& handlePing(IRCMessage& msg);
};

#endif  // __COMMAND_HANDLER_HPP__
