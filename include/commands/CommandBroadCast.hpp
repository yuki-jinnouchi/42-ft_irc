#pragma once
#ifndef __COMMAND_BROAD_CAST_HPP__
#define __COMMAND_BROAD_CAST_HPP__

#include "ACommand.hpp"

class CommandBroadCast : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandBroadCast(IRCServer* server);
  ~CommandBroadCast();
  // CommandCap(const CommandCap& other);
  // CommandCap& operator=(const CommandCap& other);

  // Member functions
  void execute(IRCMessage& msg);
};

#endif  // __COMMAND_BROAD_CAST_HPP__
