#include "CommandNick.hpp"

/*
  @brief IRC command "NICK" handler.
*/

CommandNick::CommandNick(IRCServer* server, std::string commandName)
    : ACommand(server, commandName) {}

CommandNick::~CommandNick() {}

void CommandNick::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setParams(msg.getParams());
  if (msg.getParam(0).empty()) {
    // TODO: バラメータが空の場合ERR_NEEDMOREPARAMS(461)では？
    reply.setErrCode(ERR_NONICKNAMEGIVEN);
    pushResponse(reply);
    return;
  }
  if (msg.getParam(0).size() > Client::kMaxNickNameSize) {
    reply.setErrCode(ERR_ERRONEUSNICKNAME);
    pushResponse(reply);
    return;
  }
  if (server_->isNickTaken(msg.getParam(0))) {
    reply.setErrCode(ERR_NICKNAMEINUSE);
    pushResponse(reply);
    return;
  }
  // TODO: valid Nick characters (432 ERR_ERRONEUSNICKNAME)
  from->setNickName(msg.getParam(0));
  registrate(msg);
  return;
}
