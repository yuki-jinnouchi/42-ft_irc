#include "Channel.hpp"

Channel::~Channel() {}

Channel::Channel(const std::string& name, Client* client)
    : name_(name), isInviteOnly_(false)
    , member_(std::set<Client*>())
    , chanop_(std::set<Client*>())
    , invited_(std::set<Client*>()) {
  member_.insert(client);
  chanop_.insert(client);
}

// Getters
const std::string& Channel::getName() const {
  return name_;
}

const std::string& Channel::getTopic() const {
  return topic_;
}

bool Channel::isInviteOnly(void) const {
  return isInviteOnly_;
}

const std::set<Client*>& Channel::getMember() const {
  return member_;
}

const std::set<Client*>& Channel::getChanop() const {
  return chanop_;
}

const std::set<Client*>& Channel::getInvited() const {
  return invited_;
}

bool Channel::isMember(Client* client) const {
  if (member_.find(client) != member_.end()) {
    return true;
  }
  return false;
}

bool Channel::isChanop(Client* client) const {
  if (chanop_.find(client) != chanop_.end()) {
    return true;
  }
  return false;
}

bool Channel::isInvited(Client* client) const {
  if (invited_.find(client) != invited_.end()) {
    return true;
  }
  return false;
}

// Setters
bool Channel::setTopic(const std::string& topic) {
  topic_ = topic;
  return true;
}

bool Channel::setPassword(const std::string& password) {
  password_ = password;
  return true;
}

bool Channel::setInviteOnly(bool isInviteOnly) {
  isInviteOnly_ = isInviteOnly;
  return true;
}

bool Channel::addMember(Client* client) {
  if (member_.find(client) != member_.end()) {
    return true;
  }
  if (!isInviteOnly_ && invited_.find(client) != invited_.end()) {
    member_.insert(client);
    return true;
  }
  return false;
}

bool Channel::addChanop(Client* client) {
  if (member_.find(client) == member_.end()) {
    return false;
  }
  if (chanop_.find(client) != chanop_.end()) {
    return true;
  }
  return false;
}

bool Channel::addInvited(Client* client) {
  invited_.insert(client);
  return true;
}

bool Channel::removeMember(Client* client) {
  if (member_.find(client) == member_.end()) {
    member_.erase(client);
    return true;
  }
  return false;
}

bool Channel::removeChanop(Client* client) {
  if (chanop_.find(client) == chanop_.end()) {
    chanop_.erase(client);
    return true;
  }
  return false;
}

bool Channel::removeInvited(Client* client) {
  if (invited_.find(client) == invited_.end()) {
    invited_.erase(client);
    return true;
  }
  return false;
}
