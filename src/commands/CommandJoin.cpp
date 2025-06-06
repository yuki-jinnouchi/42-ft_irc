#include "CommandJoin.hpp"

CommandJoin::CommandJoin(IRCServer* server) : ACommand(server, "JOIN") {}

CommandJoin::~CommandJoin() {}

void CommandJoin::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  if (!validJoin(msg)) return;

  std::string channelName = msg.getParam(0);
  Channel* channel = server_->getChannel(channelName);
  // If channel does not exist, create a new one
  if (!channel) {
    addClientToNewChannel(msg);
    return;
  }
  // If already a member of the channel, just ignore
  if (channel->isMember(from)) return;
  // Check if the channel is full
  int userLimit = channel->getUserLimit();
  if (userLimit > 0 &&
      channel->getMember().size() >= static_cast<size_t>(userLimit)) {
    reply.addParam(channelName);
    reply.setResCode(ERR_CHANNELISFULL);
    pushResponse(reply);
    return;
  }
  // Check if the channel is invite-only and if invited
  if (channel->getIsInviteOnly() && !channel->isInvited(from)) {
    reply.addParam(channelName);
    reply.setResCode(ERR_INVITEONLYCHAN);
    pushResponse(reply);
    return;
  }
  channel->addMember(from);
  channel->removeInvited(from);
  sendResponseToChannel(msg);
  sendResponceToFrom(msg);
}

bool CommandJoin::validJoin(IRCMessage& msg) {
  if (!checkIsRegistered(msg)) return false;
  if (!checkParamNum(msg, 1)) return false;

  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  if (2 < msg.getParams().size()) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return false;
  }
  std::string channelName = msg.getParam(0);
  if (channelName[0] != '#' && channelName[0] != '&') {
    reply.addParam(channelName);
    reply.setResCode(ERR_NOSUCHCHANNEL);
    pushResponse(reply);
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
  sendResponceToFrom(msg);
}

void CommandJoin::sendResponseToChannel(IRCMessage& msg) {
  std::string channelName = msg.getParam(0);
  Channel* channel = server_->getChannel(channelName);
  Client* from = msg.getFrom();
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("JOIN");
    reply.addParam(channelName);
    pushResponse(reply);
  }
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
  IRCMessage nameReply(from, from);
  nameReply.setResCode(RPL_NAMREPLY);
  nameReply.addParam(channelName);
  std::string nameList = "";
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    if (!nameList.empty()) nameList += " ";
    nameList += (*it)->getNickName();
  }
  nameReply.addParam(nameList);
  pushResponse(nameReply);

  // Reply RPL_ENDOFNAMES
  IRCMessage endOfNamesReply(from, from);
  endOfNamesReply.setResCode(RPL_ENDOFNAMES);
  endOfNamesReply.addParam(channelName);
  pushResponse(endOfNamesReply);
}
