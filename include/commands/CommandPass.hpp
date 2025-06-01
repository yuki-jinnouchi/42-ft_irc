#pragma once
#ifndef __COMMAND_PASS_HPP__
#define __COMMAND_PASS_HPP__

#include "ACommand.hpp"

class CommandPass : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandPass(IRCServer* server);
  ~CommandPass();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandPass();
  CommandPass(const CommandPass& other);
  CommandPass& operator=(const CommandPass& other);
};

#endif  // __COMMAND_PASS_HPP__
