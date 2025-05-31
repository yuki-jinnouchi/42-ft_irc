#include "CommandUser.hpp"

/*
  @brief IRC command "USER" handler.
*/

CommandUser::CommandUser(IRCServer* server, std::string commandName)
    : ACommand(server, commandName) {}

CommandUser::~CommandUser() {}

void CommandUser::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::vector<Client*> clients;
  clients.push_back(from);
  if (msg.getParam(0).empty() || msg.getParam(3).empty()) {
    addResponseByCode(clients, ERR_NEEDMOREPARAMS);
    return;
  }
  if (from->getIsRegistered()) {
    addResponseByCode(clients, ERR_ALREADYREGISTRED);
    return;
  }
  // TODO: Valid入れる？
  from->setUserName(msg.getParam(0));
  from->setRealName(msg.getParam(3));
  registrate(msg);
}
