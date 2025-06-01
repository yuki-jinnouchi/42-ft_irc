#pragma once
#ifndef __COMMAND_PING_HPP__
#define __COMMAND_PING_HPP__

#include "ACommand.hpp"

class CommandPing : public ACommand {
 public:
  // Member variables
  IRCServer* server_;  // Serverオブジェクトへのポインタ

  // Orthodox Canonical Form
  CommandPing(IRCServer* server);
  ~CommandPing();
  // CommandPing(const CommandPing& other);
  // CommandPing& operator=(const CommandPing& other);

  // Member functions
  void execute(IRCMessage& msg);
};

#endif  // __COMMAND_PING_HPP__
