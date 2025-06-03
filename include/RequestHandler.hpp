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

  // Member functions
  ACommand* getCommand(const std::string& commandName);
  void execCommand(IRCMessage& msg);

  RequestHandler();
  RequestHandler(const RequestHandler& other);
  RequestHandler& operator=(const RequestHandler& other);

 public:
  // Orthodox Canonical Form
  RequestHandler(IRCServer* server);
  ~RequestHandler();

  // Member functions
  void handleCommand(IRCMessage& msg);
};

#endif  // __REQUEST_HANDLER_HPP__
