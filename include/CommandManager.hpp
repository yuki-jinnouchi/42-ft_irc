#pragma once
#ifndef __COMMAND_MANAGER_HPP__
#define __COMMAND_MANAGER_HPP__

// #include "IRCMessage.hpp"
// #include "IRCServer.hpp"

#include <map>

#include "ACommand.hpp"
// #include "CommandCap.hpp"
// #include "CommandNick.hpp"
#include "CommandPass.hpp"
// #include "CommandUser.hpp"

class IRCServer;
class IRCMessage;
class ACommand;

// enum CommandType {
//   CAP,
//   PASS,
//   NICK,
//   USER,
//   JOIN,
//   PART,
//   PRIVMSG,
//   QUIT,
//   PING,
//   PONG
// };

class CommandManager {
 private:
  // Member variables
  std::map<std::string, ACommand*> commands_;
  IRCServer* server_;

  // Getters
  ACommand* getCommand(const std::string& commandName);

  // Member functions
  void registerCommands();
  void unregisterCommands();

  public:
  // Orthodox Canonical Form
  // CommandManager();
  CommandManager(IRCServer* server);
  ~CommandManager();
  CommandManager(const CommandManager& other);
  CommandManager& operator=(const CommandManager& other);

  // Member functions
  void execCommand(IRCMessage& msg);
};

#endif  // __COMMAND_MANAGER_HPP__
