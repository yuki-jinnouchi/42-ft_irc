#pragma once
#ifndef __COMMAND_BROAD_CAST_HPP__
#define __COMMAND_BROAD_CAST_HPP__

#include "ACommand.hpp"

class CommandBroadCast : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandBroadCast(IRCServer* server);
  ~CommandBroadCast();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandBroadCast();
  CommandBroadCast(const CommandBroadCast& other);
  CommandBroadCast& operator=(const CommandBroadCast& other);
};

#endif  // __COMMAND_BROAD_CAST_HPP__
