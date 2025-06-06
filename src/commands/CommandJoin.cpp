#include "CommandJoin.hpp"

CommandJoin::CommandJoin(IRCServer* server) : ACommand(server, "JOIN") {}

CommandJoin::~CommandJoin() {}

void CommandJoin::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  std::string channelName = msg.getParam(0);
  if (channelName[0] != '#' && channelName[0] != '&') {
    reply.addParam(channelName);
    reply.setResCode(ERR_NOSUCHCHANNEL);
    pushResponse(reply);
    return;
  }

  Channel* channel = server_->getChannel(channelName);
  if (!channel) {
    addClientToNewChannel(msg);
    return;
  } else {
    if (channel->isMember(from)) {
      // reply.addParam(channelName);
      // reply.setResCode(ERR_USERONCHANNEL);
      // pushResponse(reply);
      return;
    }
    channel->addMember(from);
    reply.setRaw(":" + from->getUserPrefix() + " JOIN " + channelName);
    pushResponse(reply);
  }
  if (channel->isMember(from)) {
    // reply.addParam(channelName);
    // reply.setResCode(ERR_USERONCHANNEL);
    // pushResponse(reply);
    return;
  }
  if (channel->getUserLimit() != 0 &&
      channel->getMember().size() >=
          static_cast<size_t>(channel->getUserLimit())) {
    reply.addParam(channelName);
    reply.setResCode(ERR_CHANNELISFULL);
    pushResponse(reply);
    return;
  }
  if (channel->getIsInviteOnly() && !channel->isInvited(from)) {
    reply.addParam(channelName);
    reply.setResCode(ERR_INVITEONLYCHAN);
    pushResponse(reply);
    return;
  }
  channel->addMember(from);
  sendResponceToFrom(msg);
  sendResponseToOthers(msg);
}

bool CommandJoin::validJoin(IRCMessage& msg) {
  if (!checkIsRegistered(msg)) {
    return false;
  }
  if (!checkParamNum(msg, 1)) {
    return false;
  }
  return true;
}

void CommandJoin::addClientToNewChannel(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  std::string channelName = msg.getParam(0);

  server_->addChannel(channelName, from);
  server_->getChannel(channelName)->addChanop(from);
  Channel* channel = server_->getChannel(channelName);
  from->addJoinedChannel(channel);
  channel->addMember(from);
  channel->addChanop(from);
  reply.setRaw(":" + from->getUserPrefix() + " JOIN " + channelName);
  pushResponse(reply);
}

void CommandJoin::sendResponceToFrom(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::string channelName = msg.getParam(0);
  Channel* channel = server_->getChannel(channelName);
  std::set<Client*> members = channel->getMember();
  // Reply RPL_TOPIC
  IRCMessage topicReply(from, from);
  topicReply.setResCode(RPL_TOPIC);
  topicReply.addParam(channelName);
  pushResponse(topicReply);
  // Reply RPL_NAMREPLY
  if (!members.empty()) {
    IRCMessage nameReply(from, from);
    nameReply.setResCode(RPL_NAMREPLY);
    nameReply.addParam(channelName);
    std::string nameList = "";
    for (std::set<Client*>::iterator it = members.begin(); it != members.end();
         ++it) {
      if (!nameList.empty()) nameList += " ";
      nameList += (*it)->getNickName();
    }
    nameReply.setBody(nameList);
    pushResponse(nameReply);
  }
  // Reply RPL_ENDOFNAMES
  IRCMessage endOfNamesReply(from, from);
  endOfNamesReply.setResCode(RPL_ENDOFNAMES);
  endOfNamesReply.addParam(channelName);
  pushResponse(endOfNamesReply);
}

void CommandJoin::sendResponseToOthers(IRCMessage& msg) {
  std::string channelName = msg.getParam(0);
  Channel* channel = server_->getChannel(channelName);
  Client* from = msg.getFrom();
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    if (to != from) {
      IRCMessage reply(from, to);
      reply.setCommand("JOIN");
      reply.setRaw(":" + from->getUserPrefix() + " JOIN " + channelName);
      pushResponse(reply);
    }
  }
}
