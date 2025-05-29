#pragma once
#ifndef __COMMAND_HANDLER_HPP__
#define __COMMAND_HANDLER_HPP__

#include <string>

#include "IRCLogger.hpp"
#include "IRCMessage.hpp"
#include "IRCServer.hpp"

class CommandHandler {
 private:
  // Member variables
  IRCServer* server_;
  // IRCMessage msg_;
  std::map<Client*, std::string> responses_;

  // Handle commands
  // void cap(IRCMessage& msg);
  void pass(IRCMessage& msg);
  void nick(IRCMessage& msg);
  void user(IRCMessage& msg);
  void join(IRCMessage& msg);
  // void Privmsg(const IRCMessage& msg);
  // void Part(const IRCMessage& msg);
  // void Quit(const IRCMessage& msg);
  void ping(IRCMessage& msg);

  // Member functions
  // void handlePong(IRCMessage& msg);
  void sendWelcome(IRCMessage& msg);

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
