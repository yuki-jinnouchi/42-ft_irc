#include "CommandPass.hpp"

/*
  @brief IRC command "PASS" handler.
  @details This class handles the "PASS" command in IRC, which is used for
           password authentication during the connection registration process.
*/

CommandPass::CommandPass(IRCServer* server, std::string commandName)
    : ACommand(server, commandName) {}

CommandPass::~CommandPass() {}

void CommandPass::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::vector<Client*> clients;
  clients.push_back(from);
  std::string password = msg.getParam(0);
  // NOTE: ignore hop count
  if (password.empty()) {
    addResponseByCode(clients, ERR_NEEDMOREPARAMS);
    return;
  }
  if (from->getIsRegistered()) {
    addResponseByCode(clients, ERR_ALREADYREGISTRED);
    return;
  }
  from->setPassword(password);
  registrate(msg);
}
