#include "CommandInvite.hpp"

CommandInvite::CommandInvite(IRCServer* server) : ACommand(server, "USER") {}

CommandInvite::~CommandInvite() {}

// Member functions
void CommandInvite::execute(IRCMessage& msg) {
  if (!validateInvite(msg)) {
    return;
  }

  Client* from = msg.getFrom();
  Client* client = server_->getClient(msg.getParam(0));
  Channel* channel = server_->getChannel(msg.getParam(1));

  channel->addInvited(client);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
  ++it) {
    Client* to = *it;
    IRCMessage reply(from, from);
    if (to == from) {
      reply.setResCode(RPL_INVITING);
      reply.addParam(client->getNickName());
      reply.addParam(channel->getName());
    } else {
      reply.setTo(to);
      reply.setCommand("INVITE");
      reply.addParam(client->getNickName());
      reply.addParam(channel->getName());
    }
    pushResponse(reply);
  }
}

bool CommandInvite::validateInvite(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  if (!checkIsRegistered(msg)) {
    return false;
  }
  if (!checkParamNum(msg, 2)) {
    return false;
  }

  Client* client = server_->getClient(msg.getParam(0));
  Channel* channel = server_->getChannel(msg.getParam(1));
  if (!client) {
    reply.setResCode(ERR_NOSUCHNICK);
    reply.addParam(msg.getParam(0));
    pushResponse(reply);
    return false;
  }
  if (!channel) {
    reply.setResCode(ERR_NOSUCHCHANNEL);
    reply.addParam(msg.getParam(1));
    pushResponse(reply);
    return false;
  }
  if (channel->isMember(client)) {
    reply.setResCode(ERR_USERONCHANNEL);
    reply.addParam(client->getNickName());
    reply.addParam(channel->getName());
    pushResponse(reply);
    return false;
  }
  if (channel->isInvited(client)) {
    return false;
  }
  return true;
}
