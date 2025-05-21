#pragma once
#ifndef __COMMAND_HANDLER_HPP__
#define __COMMAND_HANDLER_HPP__

#include <string>

#include "IRCMessage.hpp"
#include "IRCServer.hpp"
#include "IRCLogger.hpp"

class CommandHandler {
 private:
  // Member variables
  IRCServer* server_;
  // IRCMessage msg_;
  std::map<Client*, std::string> responses_;
  
  // Handle commands
  void Pass(const IRCMessage& msg);
  void Nick(const IRCMessage& msg);
  void User(const IRCMessage& msg);
  void Join(const IRCMessage& msg);
  // void Privmsg(const IRCMessage& msg);
  // void Part(const IRCMessage& msg);
  // void Quit(const IRCMessage& msg);
  void Ping(IRCMessage& msg);
  // void handlePong(IRCMessage& msg);

 public:
  // Orthodox Canonical Form
  // CommandHandler();
  CommandHandler(IRCServer* server);
  ~CommandHandler();
  CommandHandler(const CommandHandler& other);
  CommandHandler& operator=(const CommandHandler& other);

  // Member functions
  const std::map<Client*, std::string>& handleCommand(IRCMessage& msg);
  const std::map<Client*, std::string>& broadCastRawMsg(IRCMessage& msg);
  // const std::map<Client*, std::string>& handlePing(IRCMessage& msg);
};

#endif  // __COMMAND_HANDLER_HPP__
