#include "CommandNick.hpp"

/*
  @brief IRC command "NICK" handler.
*/

#include "IRCParser.hpp"

CommandNick::CommandNick(IRCServer* server) : ACommand(server, "NICK") {}

CommandNick::~CommandNick() {}

bool CommandNick::validateNickName(IRCMessage& msg, IRCMessage& reply) {
  // 引数なし
  if (msg.getParam(0).empty()) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return false;
  }
  // 9文字以上
  if (msg.getParam(0).size() > Client::kMaxNickNameSize) {
    reply.setResCode(ERR_ERRONEUSNICKNAME);
    reply.setBody(msg.getParam(0) + " :Nickname too long, max. 9 characters");
    pushResponse(reply);
    return false;
  }
  // 不正な文字列の場合
  if (!IRCParser::isValidNickName(msg.getParam(0))) {
    reply.setResCode(ERR_ERRONEUSNICKNAME);
    pushResponse(reply);
    return false;
  }
  // 既に存在
  if (server_->isNickTaken(msg.getParam(0))) {
    if (reply.getFrom()->getNickName() == msg.getParam(0)) {
      // 自分自身と同じ場合、何もしない
      return false;
    }
    reply.setResCode(ERR_NICKNAMEINUSE);
    pushResponse(reply);
    return false;
  }
  return true;
}

void CommandNick::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setParams(msg.getParams());

  if (!validateNickName(msg, reply)) {
    return;
  }
  // 更新前のニックネームでプレフィックスを設定
  std::string prefix = from->getUserPrefix();

  from->setNickName(msg.getParam(0));
  registrate(msg);

  // TODO 未ログインの場合は返信しない

  reply.setRaw(":" + prefix + " NICK :" + from->getNickName());
  pushResponse(reply);

  // TODO : チャンネルに参加している他のユーザーに通知する
  return;
}
