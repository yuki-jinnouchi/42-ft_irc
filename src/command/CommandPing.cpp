#include "command/CommandPing.hpp"

#include "IRCLogger.hpp"
#include "IRCServer.hpp"

CommandPing::CommandPing(IRCServer* server) : ACommand(server, "PING") {}
CommandPing::~CommandPing() {}

void CommandPing::execute(IRCMessage& msg) {
  DEBUG_MSG("CommandPing: Executing PING command.");
  IRCMessage reply;
  reply.setFrom(msg.getFrom());
  reply.setTo(msg.getFrom());

  msg.getFrom()->pushSendingMsg("PONG " + msg.getParam(0) + "\r\n");
  server_->insertSendQueue(reply.getTo());
}
