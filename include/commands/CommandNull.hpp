#pragma once
#ifndef __COMMAND_NULL_HPP__
#define __COMMAND_NULL_HPP__

#include "ACommand.hpp"

class CommandNull : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandNull(IRCServer* server);
  ~CommandNull();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandNull();
  CommandNull(const CommandNull& other);
  CommandNull& operator=(const CommandNull& other);
};

#endif  // __COMMAND_NULL_HPP__
