#include "CommandPrivMsg.hpp"

/*
  @brief IRC command "NICK" handler.
*/

#include "IRCParser.hpp"

CommandPrivMsg::CommandPrivMsg(IRCServer* server)
    : ACommand(server, "PRIVMSG") {}

CommandPrivMsg::~CommandPrivMsg() {}

bool CommandPrivMsg::validatePrivMsg(IRCMessage& msg, IRCMessage& reply) {
  // 未ログインの場合
  if (!msg.getFrom()->getIsRegistered()) {
    reply.setResCode(ERR_NOTREGISTERED);
    pushResponse(reply);
    return false;
  }
  // 引数なし
  if (msg.getParam(0).empty()) {
    reply.setResCode(ERR_NORECIPIENT);
    pushResponse(reply);
    return false;
  }
  // 引数が1つだけ
  if (msg.getParams().size() == 1) {
    reply.setResCode(ERR_NOTEXTTOSEND);
    pushResponse(reply);
    return false;
  }
  // 引数が2つより多い
  if (msg.getParams().size() > 2) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return false;
  }
  return true;
}

void CommandPrivMsg::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setParams(msg.getParams());

  if (!validatePrivMsg(msg, reply)) {
    return;
  }
  Channel* channel = server_->getChannel(msg.getParam(0));
  Client* to = server_->getClient(msg.getParam(0));
  // 存在しないニックネーム/チャンネル、存在するが相手が未ログインの場合
  if ((channel == NULL && to == NULL) ||
      (to != NULL && !to->getIsRegistered())) {
    reply.setResCode(ERR_NOSUCHNICK);
    return pushResponse(reply);
  }
  if (to != NULL) {
    // 存在するニックネームの場合
    reply.setTo(to);
    reply.setRaw(":" + from->getUserPrefix() + " PRIVMSG " + to->getNickName() +
                 " :" + msg.getParam(1));
    return pushResponse(reply);
  }

  // TODO チャンネルに送信する場合
  return;
}
