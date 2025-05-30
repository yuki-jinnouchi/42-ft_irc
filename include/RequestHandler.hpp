#pragma once
#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <string>

#include "IRCLogger.hpp"
#include "IRCMessage.hpp"
// #include "IRCServer.hpp"
#include "command/ACommand.hpp"

// class IRCServer;
class RequestHandler {
 private:
  // Member variables
  IRCServer* server_;
  std::map<std::string, ACommand*> commands_;

  void initializeCommands(IRCServer* server_);

  RequestHandler(const RequestHandler& other);
  RequestHandler& operator=(const RequestHandler& other);

 public:
  // Orthodox Canonical Form
  // RequestHandler();
  RequestHandler(IRCServer* server);
  ~RequestHandler();

  // Member functions
  void handleCommand(IRCMessage& msg);
  // const std::map<Client*, std::string>& broadCastRawMsg(IRCMessage& msg);
};

#endif  // __REQUEST_HANDLER_HPP__
