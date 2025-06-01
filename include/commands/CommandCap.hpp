#pragma once
#ifndef __COMMAND_CAP_HPP__
#define __COMMAND_CAP_HPP__

#include "ACommand.hpp"

class CommandCap : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandCap(IRCServer* server, std::string commandName);
  ~CommandCap() {}
  // CommandCap(const CommandCap& other);
  // CommandCap& operator=(const CommandCap& other);

  // Member functions
  void execute(IRCMessage& msg);
  void commandCapList(IRCMessage& reply);
  void commandCapAck(IRCMessage& msg, IRCMessage& reply);
  void commandCapEnd(IRCMessage& reply);
};

#endif  // __COMMAND_CAP_HPP__
