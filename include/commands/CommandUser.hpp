#pragma once
#ifndef __COMMAND_USER_HPP__
#define __COMMAND_USER_HPP__

#include "ACommand.hpp"

class CommandUser : public ACommand {
 public:
  // Member variables
  IRCServer* server_;  // Serverオブジェクトへのポインタ

  // Orthodox Canonical Form
  CommandUser(IRCServer* server);
  ~CommandUser();
  // CommandUser(const CommandUser& other);
  // CommandUser& operator=(const CommandUser& other);

  // Member functions
  void execute(IRCMessage& msg);
};

#endif  // __COMMAND_USER_HPP__
