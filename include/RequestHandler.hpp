#pragma once
#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <string>

#include "IRCLogger.hpp"
#include "IRCMessage.hpp"
#include "IRCServer.hpp"

class RequestHandler {
 private:
  // Member variables
  IRCServer* server_;
  std::map<Client*, std::string> responses_;

  // Member functions
  void sendWelcome(IRCMessage& msg);

 public:
  // Orthodox Canonical Form
  // RequestHandler();
  RequestHandler(IRCServer* server);
  ~RequestHandler();
  RequestHandler(const RequestHandler& other);
  RequestHandler& operator=(const RequestHandler& other);

  // Member functions
  const std::map<Client*, std::string>& handleCommand(IRCMessage& msg);
  const std::map<Client*, std::string>& broadCastRawMsg(IRCMessage& msg);
};

#endif  // __REQUEST_HANDLER_HPP__
