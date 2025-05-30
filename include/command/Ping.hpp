#pragma once
#ifndef __PING_HPP__
#define __PING_HPP__

#include "command/ACommand.hpp"

class Ping : public ACommand {
 public:
  Ping(IRCServer* server);
  ~Ping();

  void execute(IRCMessage& msg);
};

#endif  // __PING_HPP__
