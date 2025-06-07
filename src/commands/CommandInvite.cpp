#include "CommandInvite.hpp"

CommandInvite::CommandInvite(IRCServer* server) : ACommand(server, "INVITE") {}

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
  // send response to from
  IRCMessage replyFrom(from, from);
  replyFrom.setResCode(RPL_INVITING);
  replyFrom.addParam(client->getNickName());
  replyFrom.addParam(channel->getName());
  pushResponse(replyFrom);
  // send response to client
  IRCMessage replyClient(from, client);
  replyClient.setCommand("INVITE");
  replyClient.addParam(client->getNickName());
  replyClient.addParam(channel->getName());
  pushResponse(replyClient);
}

bool CommandInvite::validateInvite(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  if (!checkIsRegistered(msg)) return false;
  if (!checkParamNum(msg, 2)) return false;
  if (msg.getParams().size() > 2) {
    reply.setResCode(ERR_NEEDMOREPARAMS);
    pushResponse(reply);
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
  if (!channel->isMember(from)) {
    reply.setResCode(ERR_NOTONCHANNEL);
    reply.addParam(channel->getName());
    pushResponse(reply);
    return false;
  }
  if (!channel->isChanop(from)) {
    reply.setResCode(ERR_CHANOPRIVSNEEDED);
    reply.addParam(channel->getName());
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
