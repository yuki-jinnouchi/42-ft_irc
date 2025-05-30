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
  std::string password = msg.getParam(0);
  // NOTE: ignore hop count
  if (password.empty()) {
    ResponseBuilder::addResponse({from}, ERR_NEEDMOREPARAMS, {"PASS"});
    return;
  }
  if (from->getIsRegistered()) {
    ResponseBuilder::addResponse({from}, ERR_ALREADYREGISTRED, {});
    return;
  }
  from->setPassword(password);
}
