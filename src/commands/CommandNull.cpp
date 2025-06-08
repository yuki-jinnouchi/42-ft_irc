#include "CommandNull.hpp"

CommandNull::CommandNull(IRCServer* server) : ACommand(server, "NULL") {}

CommandNull::~CommandNull() {}

void CommandNull::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (!from->getIsRegistered()) return;
  IRCMessage reply(from, from);
  reply.setResCode(ERR_UNKNOWNCOMMAND);
  reply.addParam(msg.getCommand());
  pushResponse(reply);
}
