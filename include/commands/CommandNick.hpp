#pragma once
#ifndef __COMMAND_NICK_HPP__
#define __COMMAND_NICK_HPP__

#include "ACommand.hpp"

class CommandNick : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandNick(IRCServer* server);
  ~CommandNick();
  // CommandNick(const CommandNick& other);
  // CommandNick& operator=(const CommandNick& other);

  // Member functions
  void execute(IRCMessage& msg);
};

#endif  // __COMMAND_NICK_HPP__
