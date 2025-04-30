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
  // void handleNick(ClientSession* client, const IRCMessage& message);
  // void handleUser(ClientSession* client, const IRCMessage& message);
  // void handleJoin(ClientSession* client, const IRCMessage& message);
  // void handlePrivmsg(ClientSession* client, const IRCMessage& message);
  // void handlePart(ClientSession* client, const IRCMessage& message);
  // void handleQuit(ClientSession* client, const IRCMessage& message);
  // void handlePing(ClientSession* client, const IRCMessage& message);
  // void handlePong(ClientSession* client, const IRCMessage& message);

 public:
  CommandHandler(IRCServer* server);
  ~CommandHandler();
  CommandHandler(const CommandHandler& other);
  CommandHandler& operator=(const CommandHandler& other);

  void handleCommand(const IRCMessage& msg);
};

#endif  // __COMMAND_HANDLER_HPP__
