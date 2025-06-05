#pragma once
#ifndef __COMMAND_PASS_HPP__
#define __COMMAND_PASS_HPP__

#include "ACommand.hpp"

/**
  @brief IRC command "PASS" handler.
  @details This class handles the "PASS" command in IRC, which is used for
           password authentication during the connection registration process.
*/

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
