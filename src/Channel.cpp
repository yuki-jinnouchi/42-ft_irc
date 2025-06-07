#include "Channel.hpp"

#include "Client.hpp"

Channel::~Channel() {}

Channel::Channel(const std::string& name, Client* client)
    : name_(name),
      topic_(""),
      key_(""),
      userLimit_(0),
      isInviteOnly_(false),
      isTopicRestricted_(false),
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

const std::string& Channel::getKey() const {
  return key_;
}

int Channel::getUserLimit() const {
  return userLimit_;
}

bool Channel::getIsInviteOnly(void) const {
  return isInviteOnly_;
}

bool Channel::getIsTopicRestricted(void) const {
  return isTopicRestricted_;
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

bool Channel::setKey(const std::string& key) {
  key_ = key;
  return true;
}

bool Channel::setUserLimit(int limit) {
  if (limit < 0) {
    return false;  // limit must be non-negative
  }
  userLimit_ = limit;
  return true;
}

bool Channel::setInviteOnly(bool isInviteOnly) {
  isInviteOnly_ = isInviteOnly;
  return true;
}

bool Channel::setTopicRestricted(bool isTopicRestricted) {
  isTopicRestricted_ = isTopicRestricted;
  return true;
}

bool Channel::addMember(Client* client) {
  if (member_.find(client) != member_.end()) return true;
  if (isInviteOnly_ && invited_.find(client) == invited_.end()) return false;
  member_.insert(client);
  client->addJoinedChannel(this);
  return true;
}

bool Channel::addChanop(Client* client) {
  if (member_.find(client) == member_.end())
    return false;
  chanop_.insert(client);
  return true;
}

bool Channel::addInvited(Client* client) {
  invited_.insert(client);
  return true;
}

bool Channel::removeMember(Client* client) {
  if (member_.find(client) == member_.end())
    return false;
  member_.erase(client);
  client->removeJoinedChannel(name_);
  return true;
}

bool Channel::removeChanop(Client* client) {
  if (chanop_.find(client) == chanop_.end())
    return false;
  chanop_.erase(client);
  return true;
}

bool Channel::removeInvited(Client* client) {
  if (invited_.find(client) == invited_.end())
    return false;
  invited_.erase(client);
  return true;
}
