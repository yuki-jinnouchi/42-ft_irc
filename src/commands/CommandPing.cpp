#include "CommandPing.hpp"

/*
  @brief IRC command "PONG" handler.
*/

CommandPing::CommandPing(IRCServer* server) : ACommand(server, "PING") {}

CommandPing::~CommandPing() {}

void CommandPing::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  // if (!from->getIsRegistered()) {
  //   return;
  // }
  reply.setRaw("PONG");
  pushResponse(reply);
}
