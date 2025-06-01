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

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandPing();
  CommandPing(const CommandPing& other);
  CommandPing& operator=(const CommandPing& other);
};

#endif  // __COMMAND_PING_HPP__
