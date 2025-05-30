#include "CommandNick.hpp"

/*
  @brief IRC command "NICK" handler.
*/

CommandNick::CommandNick(IRCServer* server, std::string commandName)
    : ACommand(server, commandName) {}

CommandNick::~CommandNick() {}

void CommandNick::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::vector<Client*> clients;
  clients.push_back(from);
  if (msg.getParam(0).empty()) {
    addResponseByCode(clients, ERR_NONICKNAMEGIVEN);
    return;
  }
  if (msg.getParam(0).size() > Client::kMaxNickNameSize) {
    addResponseByCode(clients, ERR_ERRONEUSNICKNAME);
    return;
  }
  if (server_->isNickTaken(msg.getParam(0))) {
    addResponseByCode(clients, ERR_NICKNAMEINUSE);
    return;
  }
  // TODO: valid Nick characters (432 ERR_ERRONEUSNICKNAME)
  from->setNickName(msg.getParam(0));
  registrate(msg);
  return;
}
