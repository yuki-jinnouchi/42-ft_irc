#include "CommandJoin.hpp"

/**
  @brief IRC command "JOIN" handler.
*/

CommandJoin::CommandJoin(IRCServer* server) : ACommand(server, "JOIN") {}

CommandJoin::~CommandJoin() {}

void CommandJoin::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  if (!checkIsRegistered(msg)) {
    return;
  }
  if (msg.getParams().empty()) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
    return;
  }
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
      reply.addParam(channelName);
      reply.setResCode(ERR_USERONCHANNEL);
      pushResponse(reply);
      return;
    }
    channel->addMember(from);
    from->addJoinedChannel(channel);
    reply.setRaw(":" + from->getUserPrefix() + " JOIN " + channelName);
    pushResponse(reply);
  }
}

void CommandJoin::addClientToNewChannel(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  std::string channelName = msg.getParam(0);

  server_->addChannel(channelName, from);
  Channel* channel = server_->getChannel(channelName);
  from->addJoinedChannel(channel);
  channel->addMember(from);
  channel->addChanop(from);
  reply.setRaw(":" + from->getUserPrefix() + " JOIN " + channelName);
  pushResponse(reply);
}
