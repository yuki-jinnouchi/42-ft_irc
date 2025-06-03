#pragma once
#ifndef __COMMAND_CAP_HPP__
#define __COMMAND_CAP_HPP__

#include "ACommand.hpp"

/**
  @brief IRC command "CAP" handler.
  @details This class handles the "CAP" command in IRC, which is used for
           capability negotiation between the client and server.
  @url https://ircv3.net/specs/core/capability-negotiation-3.1.html
*/

class CommandCap : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandCap(IRCServer* server);
  ~CommandCap();

  // Member functions
  void execute(IRCMessage& msg);
  void commandCapList(IRCMessage& reply);
  void commandCapAck(IRCMessage& msg, IRCMessage& reply);
  void commandCapEnd(IRCMessage& reply);

 private:
  CommandCap();
  CommandCap(const CommandCap& other);
  CommandCap& operator=(const CommandCap& other);
};

#endif  // __COMMAND_CAP_HPP__
