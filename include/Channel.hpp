#pragma once
#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <set>
#include <string>

#include "Client.hpp"

class Client;

class Channel {
 private:
  // Member variables
  std::string name_;
  std::string topic_;
  std::string password_;
  bool isInviteOnly_;
  std::set<Client*> member_;
  std::set<Client*> chanop_;
  std::set<Client*> invited_;

 public:
  // Orthodox Cannonical Form
  // Channel();
  ~Channel();
  // Channel(const Channel& other);
  // Channel& operator=(const Channel& other);
  Channel(const std::string& name, Client* client);

  // Getters
  const std::string& getName() const;
  const std::string& getTopic() const;
  bool isInviteOnly() const;
  const std::set<Client*>& getMember() const;
  const std::set<Client*>& getChanop() const;
  const std::set<Client*>& getInvited() const;
  bool isMember(Client* client) const;
  bool isChanop(Client* client) const;
  bool isInvited(Client* client) const;

  // Setters
  bool setName(const std::string& name);
  bool setTopic(const std::string& topic);
  bool setPassword(const std::string& password);
  bool setInviteOnly(bool isInviteOnly);
  bool addMember(Client* client);
  bool addChanop(Client* client);
  bool addInvited(Client* client);
  bool removeMember(Client* client);
  bool removeChanop(Client* client);
  bool removeInvited(Client* client);

};

#endif
