#pragma once
#ifndef __COMMAND_JOIN_HPP__
#define __COMMAND_JOIN_HPP__

#include "ACommand.hpp"

class CommandJoin : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandJoin(IRCServer* server);
  ~CommandJoin();

  // Member functions
  void execute(IRCMessage& msg);
  void addClientToNewChannel(IRCMessage& msg);
  void addClientToExistingChannel(IRCMessage& msg);

 private:
  CommandJoin();
  CommandJoin(const CommandJoin& other);
  CommandJoin& operator=(const CommandJoin& other);
};

#endif  // __COMMAND_JOIN_HPP__
