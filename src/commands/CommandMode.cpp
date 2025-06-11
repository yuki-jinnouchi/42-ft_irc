#include "CommandMode.hpp"

#include "cstdlib"

CommandMode::CommandMode(IRCServer* server) : ACommand(server, "MODE") {}

CommandMode::~CommandMode() {}


// Member functions
void CommandMode::execute(IRCMessage& msg) {
  if (!validateMode(msg)) return;

  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setParams(msg.getParams());
  Channel* channel = server_->getChannel(msg.getParam(0));

  std::string mode = msg.getParam(1);
  char mode_direction = mode[0];
  std::string mode_flags = mode.substr(1);
  reply.setCommand("MODE");
  int param_count = 2;
  for (std::string::iterator it = mode_flags.begin(); it != mode_flags.end();
       ++it) {
    char flag = *it;
    if (mode_direction == '+' && flag == 'i') {
      plusI(channel, from);
    } else if (mode_direction == '-' && flag == 'i') {
      minusI(channel, from);
    } else if (mode_direction == '+' && flag == 't') {
      plusT(channel, from);
    } else if (mode_direction == '-' && flag == 't') {
      minusT(channel, from);
    } else if (mode_direction == '+' && flag == 'k') {
      plusK(channel, from, msg.getParam(param_count++));
    } else if (mode_direction == '-' && flag == 'k') {
      minusK(channel, from);
    } else if (mode_direction == '+' && flag == 'o') {
      Client* client = server_->getClient(msg.getParam(param_count++));
      if (!validClient(msg, client)) continue;
      plusO(channel, from, client);
    } else if (mode_direction == '-' && flag == 'o') {
      Client* client = server_->getClient(msg.getParam(param_count++));
      if (!validClient(msg, client)) continue;
      minusO(channel, from, client);
    } else if (mode_direction == '+' && flag == 'l') {
      std::string str_limit = msg.getParam(param_count++);
      plusL(channel, from, str_limit);
    } else if (mode_direction == '-' && flag == 'l') {
      minusL(channel, from);
    } else {
      reply.setResCode(ERR_UNKNOWNMODE);
      reply.addParam(std::string(1, flag));
      pushResponse(reply);
    }
  }
}

bool CommandMode::validateMode(IRCMessage& msg) {
  if (!checkIsRegistered(msg)) return false;
  if (!checkParamNum(msg, 2)) return false;

  Client* from = msg.getFrom();
  IRCMessage reply(from, from);

  std::string dest = msg.getParam(0);
  Channel* channel = server_->getChannel(dest);
  if ((dest[0] != '#' && dest[0] != '&') || !channel) {
    // // MODE <username> の場合は一旦無視
    // reply.setResCode(ERR_NOSUCHCHANNEL);
    // reply.addParam(dest);
    // pushResponse(reply);
    return false;
  }
  if (!channel){
    reply.setResCode(ERR_NOSUCHCHANNEL);
    reply.addParam(dest);
    pushResponse(reply);
    return false;
  }
  if (!channel->isChanop(from) && !channel->isMember(from)) {
    reply.setResCode(ERR_CHANOPRIVSNEEDED);
    reply.addParam(channel->getName());
    return false;
  }
  return true;
}

bool CommandMode::validClient(IRCMessage& msg, Client* client) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  if (!client) {
    reply.setResCode(ERR_NOSUCHNICK);
    reply.addParam(msg.getParam(0));
    pushResponse(reply);
    return false;
  }
  if (!client->getIsRegistered()) {
    reply.setResCode(ERR_NOTREGISTERED);
    pushResponse(reply);
    return false;
  }
  return true;
}

// Mode commands
bool CommandMode::plusI(Channel* channel, Client* from) {
  if (channel->getIsInviteOnly()) return false;
  channel->setInviteOnly(true);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("+i");
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::minusI(Channel* channel, Client* from) {
  if (!channel->getIsInviteOnly()) return false;
  channel->setInviteOnly(false);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("-i");
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::plusT(Channel* channel, Client* from) {
  if (channel->getIsTopicRestricted()) return false;
  channel->setTopicRestricted(true);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("+t");
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::minusT(Channel* channel, Client* from) {
  if (!channel->getIsTopicRestricted()) return false;
  channel->setTopicRestricted(false);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("-t");
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::plusK(Channel* channel, Client* from,
                        const std::string& key) {
  if (channel->getKey() == key) return false;
  channel->setKey(key);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("+k " + key);
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::minusK(Channel* channel, Client* from) {
  if (channel->getKey().empty()) return false;
  channel->setKey("");
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("-k");
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::plusO(Channel* channel, Client* from, Client* client) {
  if (!channel->isMember(client)) {
    IRCMessage reply(from, from);
    reply.setResCode(ERR_USERNOTINCHANNEL);
    reply.addParam(client->getNickName());
    reply.addParam(channel->getName());
    pushResponse(reply);
    return false;
  }
  if (channel->isChanop(client)) return false;
  channel->addChanop(client);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("+o " + client->getNickName());
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::minusO(Channel* channel, Client* from, Client* client) {
  if (!channel->isMember(client)) {
    IRCMessage reply(from, from);
    reply.setResCode(ERR_USERNOTINCHANNEL);
    reply.addParam(client->getNickName());
    reply.addParam(channel->getName());
    pushResponse(reply);
    return false;
  }
  if (!channel->isChanop(client)) return false;
  channel->removeChanop(client);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("-o " + client->getNickName());
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::plusL(Channel* channel, Client* from, std::string limit) {
  int int_limit = std::atoi(limit.c_str());
  if (int_limit <= 0) return false;
  channel->setUserLimit(int_limit);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("+l " + limit);
    pushResponse(reply);
  }
  return true;
}

bool CommandMode::minusL(Channel* channel, Client* from) {
  if (channel->getUserLimit() == 0) return false;
  channel->setUserLimit(0);
  std::set<Client*> members = channel->getMember();
  for (std::set<Client*>::iterator it = members.begin(); it != members.end();
       ++it) {
    Client* to = *it;
    IRCMessage reply(from, to);
    reply.setCommand("MODE");
    reply.addParam(channel->getName());
    reply.addParam("-l");
    pushResponse(reply);
  }
  return true;
}
