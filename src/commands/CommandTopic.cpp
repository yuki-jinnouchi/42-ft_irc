#include "CommandTopic.hpp"

CommandTopic::CommandTopic(IRCServer* server)
    : ACommand(server, "TOPIC") {}

CommandTopic::~CommandTopic() {}

bool CommandTopic::validateTopic(IRCMessage& msg, IRCMessage& reply) {
  // 引数なし
  if (msg.getParam(0).empty()) {
    reply.setResCode(ERR_NORECIPIENT);
    pushResponse(reply);
    return false;
  }
  // 引数が1つより多い
  if (msg.getParams().size() > 2) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return false;
  }
  return true;
}

void CommandTopic::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setParams(msg.getParams());
  std::string channel_name = msg.getParam(0);
  std::string topic = msg.getParam(1);

  if (!validateTopic(msg, reply)) {
    return;
  }
  Channel* channel = server_->getChannel(channel_name);
  if (!channel) {
    reply.setResCode(ERR_NOSUCHCHANNEL);
    reply.addParam(channel_name);
    pushResponse(reply);
    return;
  }
  if (topic.empty()) {
    // トピックが空の場合、現在のトピックを取得
    reply.setResCode(RPL_TOPIC);
    reply.addParam(channel->getTopic());
    pushResponse(reply);
    return;
  }
  // トピックを設定
  if (!channel->isChanop(from)) {
    reply.setResCode(ERR_CHANOPRIVSNEEDED);
    reply.addParam(channel_name);
    pushResponse(reply);
    return;
  }
  channel->setTopic(topic);
  reply.setResCode(RPL_TOPIC);
  reply.addParam(channel->getTopic());
  pushResponse(reply);
}
