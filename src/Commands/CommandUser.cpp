#include "CommandUser.hpp"

/*
  @brief IRC command "PASS" handler.
  @details This class handles the "PASS" command in IRC, which is used for
           password authentication during the connection registration process.
*/

CommandUser::CommandUser(IRCServer* server, std::string commandName)
    : ACommand(server, commandName) {}

CommandUser::~CommandUser() {}

void CommandUser::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (msg.getParam(0).empty() || msg.getParam(3).empty()) {
    ResponseBuilder::addResponse({from}, ERR_NEEDMOREPARAMS, {"USER"});
    return;
  }
  if (from->getIsRegistered()) {
    ResponseBuilder::addResponse({from}, ERR_ALREADYREGISTRED, {});
    return;
  }
  // TODO: Valid入れる？
  from->setUserName(msg.getParam(0));
  from->setRealName(msg.getParam(3));
}
