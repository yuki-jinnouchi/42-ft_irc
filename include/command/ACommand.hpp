#pragma once
#ifndef __ACOMMAND_HPP__
#define __ACOMMAND_HPP__

#include <string>

#include "IRCMessage.hpp"

class IRCServer;
class ACommand {
 protected:
  // Member variables
  IRCServer* server_;
  std::string command_name_;

  // Member functions
  // void addResponse(std::vector<Client*> Clients, ResponseCode responseCode);
  // std::string formatResponse(int const responseCode, std::string const&
  // fromat,
  //                            std::vector<std::string> const& values);
  // std::string generateResponse(int responseCode);

  ACommand();
  ACommand(const ACommand& other);
  ACommand& operator=(const ACommand& other);

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
