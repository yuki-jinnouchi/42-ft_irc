#include "CommandPing.hpp"

CommandPing::CommandPing(IRCServer* server) : ACommand(server, "PING") {}

CommandPing::~CommandPing() {}

void CommandPing::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  // TODO
  // if (!from->getIsRegistered()) {
  //   return;
  // }
  if (msg.getParams().empty()) {
    reply.setResCode(ERR_NOORIGIN);
    pushResponse(reply);
    return;
  }

  reply.setRaw(":" + server_->getServerName() + " PONG " +
               server_->getServerName() + " :" + msg.getParam(0));

  pushResponse(reply);
}
