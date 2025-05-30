#include "CommandPing.hpp"

/*
  @brief IRC command "PONG" handler.
*/

CommandPing::CommandPing(IRCServer* server, std::string commandName)
    : ACommand(server, commandName) {}

CommandPing::~CommandPing() {}

void CommandPing::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::vector<Client*> clients;
  clients.push_back(from);
  // if (!from->getIsRegistered()) {
  //   return;
  // }
  addResponseText(from, "PONG");
}
