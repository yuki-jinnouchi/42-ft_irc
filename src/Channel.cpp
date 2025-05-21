#include "Channel.hpp"

Channel::~Channel() {}

Channel::Channel(const std::string& name, Client* client)
    : name_(name),
      isInviteOnly_(false),
      member_(std::set<Client*>()),
      chanop_(std::set<Client*>()),
      invited_(std::set<Client*>()) {
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
bool Channel::setName(const std::string& name) {
  name_ = name;
}

bool Channel::setTopic(const std::string& topic) {
  topic_ = topic;
}

bool Channel::setPassword(const std::string& password) {
  password_ = password;
}

bool Channel::setInviteOnly(bool isInviteOnly) {
  isInviteOnly_ = isInviteOnly;
}

bool Channel::addMember(Client* client) {
  member_.insert(client);
}

bool Channel::addChanop(Client* client) {
  chanop_.insert(client);
}

bool Channel::addInvited(Client* client) {
  invited_.insert(client);
}

bool Channel::removeMember(Client* client) {
  member_.erase(client);
}

bool Channel::removeChanop(Client* client) {
  chanop_.erase(client);
}

bool Channel::removeInvited(Client* client) {
  invited_.erase(client);
}

