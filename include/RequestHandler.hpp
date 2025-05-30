#pragma once
#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <string>

#include "IRCLogger.hpp"
#include "IRCMessage.hpp"

class IRCServer;
class ACommand;

class RequestHandler {
 private:
  // Member variables
  IRCServer* server_;
  std::map<std::string, ACommand*> commands_;

  // Initialize/Delete Command List (commands_)
  void registerCommands();
  void unregisterCommands();

  // Member functions
  ACommand* getCommand(const std::string& commandName);
  void execCommand(IRCMessage& msg);
  void sendWelcome(IRCMessage& msg);

 public:
  // Orthodox Canonical Form
  // RequestHandler();
  RequestHandler(IRCServer* server);
  ~RequestHandler();
  RequestHandler(const RequestHandler& other);
  RequestHandler& operator=(const RequestHandler& other);

  // Member functions
  void handleCommand(IRCMessage& msg);
  const std::map<Client*, std::string>& broadCastRawMsg(IRCMessage& msg);
};

#endif  // __REQUEST_HANDLER_HPP__
