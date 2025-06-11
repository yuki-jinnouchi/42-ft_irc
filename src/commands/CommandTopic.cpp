#include "CommandTopic.hpp"

CommandTopic::CommandTopic(IRCServer* server) : ACommand(server, "TOPIC") {}

CommandTopic::~CommandTopic() {}

bool CommandTopic::validateTopic(IRCMessage& msg) {
  // ログインチェック
  if (!checkIsRegistered(msg)) {
    return false;
  }
  // パラメータの数をチェック
  if (!checkParamNum(msg, 1, 2)) {
    return false;
  }
  return true;
}

Channel* CommandTopic::getAndValidateChannel(IRCMessage& msg) {
  IRCMessage reply(msg.getFrom(), msg.getFrom());
  reply.addParam(msg.getParam(0));

  Channel* channel = server_->getChannel(msg.getParam(0));
  if (!channel) {
    reply.setResCode(ERR_NOSUCHCHANNEL);
    pushResponse(reply);
    return NULL;
  }
  if (!channel->isMember(msg.getFrom())) {
    reply.setResCode(ERR_NOTONCHANNEL);
    pushResponse(reply);
    return NULL;
  }
  return channel;
}

void CommandTopic::execute(IRCMessage& msg) {
  if (!validateTopic(msg)) {
    return;
  }

  std::string topic = msg.getParam(1);

  Channel* channel = getAndValidateChannel(msg);
  if (channel == NULL) {
    return;
  }

  IRCMessage reply(msg.getFrom(), msg.getFrom());
  reply.addParam(channel->getName());
  if (topic.empty()) {
    if (channel->getTopic().empty()) {
      // トピックがない場合
      reply.setResCode(RPL_NOTOPIC);
    } else {
      // 現在のトピックを返す
      reply.setResCode(RPL_TOPIC);
      reply.addParam(channel->getTopic());
    }
    pushResponse(reply);
    return;
  }

  // TODO チャンネルがtモード、かつ権限がない場合　
  if (channel->getIsTopicRestricted() && !channel->isChanop(msg.getFrom())) {
    reply.setResCode(ERR_CHANOPRIVSNEEDED);
    pushResponse(reply);
    return;
  }

  // トピックを設定
  channel->setTopic(topic);

  // 全てのユーザーにトピックの変更を通知
  reply.setCommand("TOPIC");
  reply.addParam(":" + channel->getTopic());
  for (std::set<Client*>::const_iterator it = channel->getMember().begin();
       it != channel->getMember().end(); ++it) {
    reply.setTo(*it);
    pushResponse(reply);
  }
}
