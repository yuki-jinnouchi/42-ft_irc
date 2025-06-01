#pragma once
#ifndef __COMMAND_NICK_HPP__
#define __COMMAND_NICK_HPP__

#include "ACommand.hpp"

class CommandNick : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandNick(IRCServer* server);
  ~CommandNick();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandNick();
  CommandNick(const CommandNick& other);
  CommandNick& operator=(const CommandNick& other);
};

#endif  // __COMMAND_NICK_HPP__
