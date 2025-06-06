#include "CommandPass.hpp"

CommandPass::CommandPass(IRCServer* server) : ACommand(server, "PASS") {}

CommandPass::~CommandPass() {}

void CommandPass::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  std::string password = msg.getParam(0);
  // NOTE: ignore hop count
  if (password.empty()) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return;
  }
  if (from->getIsRegistered()) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return;
  }
  from->setPassword(password);
  registrate(msg);
}
