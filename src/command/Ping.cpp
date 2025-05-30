#include "command/Ping.hpp"

Ping::Ping(IRCServer* server) : ACommand(server) {}
Ping::~Ping() {}

void Ping::execute(IRCMessage& msg) {
  IRCMessage reply;
  reply.setTo(msg.getFrom());

  if (msg.getParam(0).empty()) {
    reply.setErrorCode(ERR_NOORIGIN);
    reply.setErrorMsg("No origin specified");
    msg.pushResponse(reply.getTo(), generateReply(msg, reply));
  }
  reply.setRaw("PONG " + msg.getParam(0));
  msg.pushResponse(reply.getTo(), generateReply(msg, reply));
}
