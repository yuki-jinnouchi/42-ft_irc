#include "CommandUser.hpp"

/*
  @brief IRC command "USER" handler.
*/

CommandUser::CommandUser(IRCServer* server) : ACommand(server, "USER") {}

CommandUser::~CommandUser() {}

void CommandUser::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setParams(msg.getParams());

  if (msg.getParam(0).empty() || msg.getParam(3).empty()) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return;
  }
  if (from->getIsRegistered()) {
    reply.setResCode(ERR_ALREADYREGISTRED);
    pushResponse(reply);
    return;
  }
  // TODO: Valid入れる？
  from->setUserName(msg.getParam(0));
  from->setRealName(msg.getParam(3));
  registrate(msg);
}
