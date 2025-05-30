#include "ACommand.hpp"

// Constructor & Destructor
ACommand::ACommand(IRCServer* server, std::string commandName)
    : server_(server), commandName_(commandName) {}

ACommand::~ACommand() {}

// Getters
const std::string& ACommand::getCommandName() const {
  return commandName_;
}

// Member functions
void ACommand::registrate(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::vector<Client*> clients = {from};
  std::vector<std::string> vals;
  if (from->getIsRegistered()) {
    vals = {"NICK", "USER"};
    ResponseBuilder::addResponse(clients, ERR_ALREADYREGISTRED, vals);
  }
  if (from->getNickName().empty() || from->getUserName().empty() ||
      from->getPassword().empty())
    return;

  if (server_->getPassword() != from->getPassword()) {
    // TODO: kickout from server?
    return;
  }
  from->setIsRegistered(true);
  vals = {from->getNickName(), ""};
  ResponseBuilder::addResponse(clients, RPL_WELCOME, vals);
}
