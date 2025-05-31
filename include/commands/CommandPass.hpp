#pragma once
#ifndef __COMMAND_PASS_HPP__
#define __COMMAND_PASS_HPP__

#include "ACommand.hpp"

class CommandPass : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandPass(IRCServer* server, std::string commandName);
  ~CommandPass();
  // CommandPass(const CommandPass& other);
  // CommandPass& operator=(const CommandPass& other);

  // Member functions
  void execute(IRCMessage& msg);
};

#endif  // __COMMAND_PASS_HPP__
