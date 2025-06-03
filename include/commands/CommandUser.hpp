#pragma once
#ifndef __COMMAND_USER_HPP__
#define __COMMAND_USER_HPP__

#include "ACommand.hpp"

class CommandUser : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandUser(IRCServer* server);
  ~CommandUser();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandUser();
  CommandUser(const CommandUser& other);
  CommandUser& operator=(const CommandUser& other);
};

#endif  // __COMMAND_USER_HPP__
