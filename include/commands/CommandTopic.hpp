#pragma once
#ifndef __COMMAND_TOPIC_HPP__
#define __COMMAND_TOPIC_HPP__

#include "ACommand.hpp"

/**
  @brief IRC command "TOPIC" handler.
*/

class CommandTopic : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandTopic(IRCServer* server);
  ~CommandTopic();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandTopic();
  CommandTopic(const CommandTopic& other);
  CommandTopic& operator=(const CommandTopic& other);

  bool validateTopic(IRCMessage& msg, IRCMessage& reply);
};

#endif  // __COMMAND_TOPIC_HPP__
