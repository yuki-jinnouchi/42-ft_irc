#pragma once
#ifndef __ACOMMAND_HPP__
#define __ACOMMAND_HPP__

#include <string>
#include <vector>

#include "IRCMessage.hpp"
#include "IRCServer.hpp"

class ACommand {
 private:
  ACommand();
  ACommand(const ACommand& other);
  ACommand& operator=(const ACommand& other);

 protected:
  // Member variables
  IRCServer* server_;
  std::string commandName_;

  // Member functions
  void pushResponse(IRCMessage& reply_msg);
  std::string generateResponseMsg(IRCMessage& reply_msg);
  std::string generateErrorMsg(IRCMessage& reply_msg);

  // Registration
  void registrate(IRCMessage& msg);

 public:
  // Orthodox Canonical Form
  ACommand(IRCServer* server, std::string commandName);
  virtual ~ACommand();

  // Getters
  const std::string& getCommandName() const;

  // Member functions
  virtual void execute(IRCMessage& msg) = 0;
};

#endif  // __ACOMMAND_HPP__
