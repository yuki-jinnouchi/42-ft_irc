#include "CommandPass.hpp"

/*
  @brief IRC command "PASS" handler.
  @details This class handles the "PASS" command in IRC, which is used for
           password authentication during the connection registration process.
*/

CommandPass::CommandPass(IRCServer* server) : ACommand(server, "PASS") {}

CommandPass::~CommandPass() {}

void CommandPass::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  std::string password = msg.getParam(0);
  // NOTE: ignore hop count
  if (password.empty()) {
    reply.setErrCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return;
  }
  if (from->getIsRegistered()) {
    reply.setErrCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return;
  }
  from->setPassword(password);
  registrate(msg);
}
