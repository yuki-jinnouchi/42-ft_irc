#include "CommandPrivMsg.hpp"

#include "Utils.hpp"

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

static void setRawPrivMsg(std::string& to_str, IRCMessage& reply) {
  reply.setRaw(":" + reply.getFrom()->getUserPrefix() + " PRIVMSG " + to_str +
               " :" + reply.getParam(1));
}

void CommandPrivMsg::sendPrivMsg(std::string& to_str, IRCMessage& reply) {
  IRCMessage msg(reply.getFrom(), reply.getFrom());
  msg.addParam(to_str);
  msg.addParam(reply.getParam(1));
  Channel* channel = server_->getChannel(to_str);
  Client* to = server_->getClient(to_str);
  // 存在しないニックネーム/チャンネル、または存在するが相手が未ログインの場合
  if ((channel == NULL && to == NULL) ||
      (to != NULL && !to->getIsRegistered())) {
    msg.setResCode(ERR_NOSUCHNICK);
    return pushResponse(msg);
  }
  if (to != NULL) {
    // ニックネームの場合
    msg.setTo(to);
    setRawPrivMsg(to_str, msg);
    return pushResponse(msg);
  }

  // チャンネルの場合
  // TODO 外部送信不可チャンネルの場合は送信不可
  // チャンネルに所属していなくてもメッセージは送信可能
  for (std::set<Client*>::const_iterator it = channel->getMember().begin();
       it != channel->getMember().end(); ++it) {
    if (*it == msg.getFrom()) {
      // 自分には送信しない
      continue;
    }
    msg.setTo(*it);
    setRawPrivMsg(to_str, msg);
    pushResponse(msg);
  }
  return;
}

void CommandPrivMsg::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setParams(msg.getParams());

  if (!validatePrivMsg(msg, reply)) {
    return;
  }
  std::vector<std::string> to_list = Utils::split(msg.getParam(0), ",");
  for (std::vector<std::string>::iterator it = to_list.begin();
       it != to_list.end(); ++it) {
    sendPrivMsg(*it, reply);
  }
  return;
}
