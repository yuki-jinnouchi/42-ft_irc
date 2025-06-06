#include "CommandKick.hpp"

#include "Utils.hpp"

CommandKick::CommandKick(IRCServer* server) : ACommand(server, "KICK") {}

CommandKick::~CommandKick() {}

bool CommandKick::validateKick(IRCMessage& msg) {
  // ログインしていない
  if (!checkIsRegistered(msg)) {
    return false;
  }
  // 引数が足りない
  if (!checkParamNum(msg, 2)) {
    return false;
  }
  // 　引数が多い
  if (msg.getParams().size() > 3) {
    IRCMessage reply(msg.getFrom(), msg.getFrom());
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return false;
  }
  return true;
}

Channel* CommandKick::getAndValidateChannel(IRCMessage& msg,
                                            const std::string& ch_str) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.addParam(ch_str);

  Channel* channel = server_->getChannel(ch_str);
  // チャンネルがない
  if (channel == NULL) {
    reply.setResCode(ERR_NOSUCHCHANNEL);
    pushResponse(reply);
    return NULL;
  }
  // 実行者がチャンネルに所属していない
  if (!channel->isMember(from)) {
    reply.setResCode(ERR_NOTONCHANNEL);
    pushResponse(reply);
    return NULL;
  }
  // 権限がない（チャンネルのオペレーターではない）
  if (!channel->isChanop(from)) {
    reply.setResCode(ERR_CHANOPRIVSNEEDED);
    pushResponse(reply);
    return NULL;
  }
  return channel;
}

Client* CommandKick::getAndValidateClient(IRCMessage& msg, Channel* channel,
                                          const std::string& nick) {
  IRCMessage reply(msg.getFrom(), msg.getFrom());
  reply.addParam(nick);
  reply.addParam(channel->getName());

  Client* client = server_->getClient(nick);
  // ニックネームが存在しない
  if (client == NULL) {
    reply.setResCode(ERR_NOSUCHNICK);
    pushResponse(reply);
    return NULL;
  }
  // 対象者がチャンネルに参加していない
  if (!channel->isMember(client)) {
    reply.setResCode(ERR_USERNOTINCHANNEL);
    pushResponse(reply);
    return NULL;
  }
  return client;
}

void CommandKick::kickMembers(IRCMessage& msg, Channel* channel,
                              const std::vector<std::string> nicks) {
  for (std::vector<std::string>::const_iterator nick = nicks.begin();
       nick != nicks.end(); ++nick) {
    Client* client = getAndValidateClient(msg, channel, *nick);
    if (client == NULL) {
      continue;
    }
    // メッセージ作成
    IRCMessage reply(msg.getFrom(), msg.getFrom());
    reply.setCommand("KICK");
    reply.addParam(channel->getName());
    reply.addParam(client->getNickName());
    if ((msg.getParams().size() == 3)) {
      reply.addParam(":" + msg.getParam(2));
    } else {
      reply.addParam(":" + msg.getFrom()->getNickName());
    }
    // チャンネル全員に通知
    for (std::set<Client*>::const_iterator it = channel->getMember().begin();
         it != channel->getMember().end(); ++it) {
      reply.setTo(*it);
      pushResponse(reply);
    }
    // チャンネルからユーザー削除
    channel->removeMember(client);
  }
}

void CommandKick::execute(IRCMessage& msg) {
  if (!validateKick(msg)) {
    return;
  }

  std::vector<std::string> ch_names = Utils::split(msg.getParam(0), ",");
  std::vector<std::string> nicks = Utils::split(msg.getParam(1), ",");
  for (std::vector<std::string>::const_iterator it = ch_names.begin();
       it != ch_names.end(); ++it) {
    Channel* channel = getAndValidateChannel(msg, *it);
    if (channel == NULL) {
      continue;
    }
    kickMembers(msg, channel, nicks);
  }
}
