#include "CommandPart.hpp"

#include "Utils.hpp"

CommandPart::CommandPart(IRCServer* server) : ACommand(server, "PART") {}

CommandPart::~CommandPart() {}

bool CommandPart::validatePart(IRCMessage& msg) {
  if (!checkIsRegistered(msg)) return false;
  if (!checkParamNum(msg, 1, 1)) return false;
  return true;
}

void CommandPart::execute(IRCMessage& msg) {
  if (!validatePart(msg)) return;
  std::string channelsParam;
  std::vector<std::string> channels = Utils::split(msg.getParam(0), ",");
  Client* from = msg.getFrom();
  for (size_t i = 0; i < channels.size(); ++i) {
    const std::string& channelName = channels[i];
    Channel* channel = server_->getChannel(channelName);
    if (!validChannel(msg, channelName)) continue;
    // チャンネルから脱退
    channel->removeMember(from);
    IRCMessage replyFrom(from, from);
    replyFrom.addParam(channelName);
    replyFrom.setCommand("PART");
    pushResponse(replyFrom);
    // チャンネルに誰もいなくなった場合、チャンネルを削除
    if (channel->getMember().empty()) {
      server_->removeChannel(channelName);
    } else {
      std::set<Client*> members = channel->getMember();
      for (std::set<Client*>::iterator it = members.begin();
           it != members.end(); ++it) {
        IRCMessage reply(from, *it);
        reply.setCommand("PART");
        reply.addParam(channelName);
        reply.setBody(msg.getBody());
        pushResponse(reply);
      }
    }
  }
}

bool CommandPart::validChannel(IRCMessage& msg,
                               const std::string& channelName) {
  Client* from = msg.getFrom();
  Channel* channel = server_->getChannel(channelName);
  if (!channel) {
    // チャンネルが存在しない場合
    IRCMessage reply(from, from);
    reply.setResCode(ERR_NOSUCHCHANNEL);
    reply.addParam(channelName);
    pushResponse(reply);
    return false;
  }
  if (!channel->isMember(from)) {
    // チャンネルに参加していない場合
    IRCMessage reply(from, from);
    reply.setResCode(ERR_NOTONCHANNEL);
    reply.addParam(channelName);
    pushResponse(reply);
    return false;
  }
  return true;
}
