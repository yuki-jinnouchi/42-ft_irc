#pragma once
#ifndef __ACOMMAND_HPP__
#define __ACOMMAND_HPP__

#include <string>
#include <vector>

#include "IRCMessage.hpp"
#include "IRCServer.hpp"

class ACommand {
 private:
  // Orthodox Canonical Form
  ACommand();
  ACommand(const ACommand& other);
  ACommand& operator=(const ACommand& other);
  std::string generateResponseMsg(IRCMessage& reply_msg);

 protected:
  // Member variables
  IRCServer* server_;
  std::string command_name_;

  // Member functions
  void pushResponse(IRCMessage& reply_msg);

  // Registration
  void registrate(IRCMessage& msg);

  // validation
  bool checkIsRegistered(IRCMessage& msg);
  bool checkParamNum(const IRCMessage& msg, size_t min_params);
  bool checkParamNum(const IRCMessage& msg, size_t min, size_t max);

 public:
  // Orthodox Canonical Form
  ACommand(IRCServer* server, std::string command_name);
  virtual ~ACommand();

  // Getters
  const std::string& getCommandName() const;

  // Member functions
  virtual void execute(IRCMessage& msg) = 0;
};

#endif  // __ACOMMAND_HPP__
