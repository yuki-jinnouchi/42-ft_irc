#include "CommandUser.hpp"

CommandUser::CommandUser(IRCServer* server) : ACommand(server, "USER") {}

CommandUser::~CommandUser() {}

bool CommandUser::validateUser(const IRCMessage& msg) {
  if (!checkParamNum(msg, 4, 4)) {
    return false;
  }

  IRCMessage reply(msg.getFrom(), msg.getFrom());
  reply.setParams(msg.getParams());

  if (msg.getFrom()->getIsRegistered()) {
    reply.setResCode(ERR_ALREADYREGISTRED);
    pushResponse(reply);
    return false;
  }

  if (!msg.getFrom()->getUserName().empty() ||
      !msg.getFrom()->getRealName().empty()) {
    reply.setResCode(ERR_NOTREGISTERED);
    pushResponse(reply);
    return false;
  }

  return true;
}

void CommandUser::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();

  if (!validateUser(msg)) {
    return;
  }

  from->setUserName(msg.getParam(0));
  from->setRealName(msg.getParam(3));
  registrate(msg);
}
