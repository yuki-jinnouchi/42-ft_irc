#pragma once
#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include <set>
#include <string>

class Client;

/**
 * @brief IRC channel class.
 * @details This class represents an IRC channel, managing its members,
 *          topic, key, user limit, and various modes.
 * @note
 * Member variables:
 * - name_: The name of the channel.
 * - topic_: The topic of the channel.
 *      initially empty.
 * - key_: The password for the channel.
 *     initially empty = no password.
 * - userLimit_: The maximum number of users allowed in the channel.
 *     initially 0 = no limit.
 * - isInviteOnly_: Whether the channel is invite-only.
 *     initially false.
 * - isTopicRestricted_: Whether only channel operators can change the topic.
 *     initially false.
 * - member_: A set of clients who are members of the channel.
 * - chanop_: A set of clients who are channel operators.
 * - invited_: A set of clients who are invited to the channel.
 */

class Channel {
 private:
  // Member variables
  std::string const name_;
  std::string topic_;
  std::string key_;
  int userLimit_;
  bool isInviteOnly_;
  bool isTopicRestricted_;

  std::set<Client*> member_;
  std::set<Client*> chanop_;
  std::set<Client*> invited_;

  Channel();
  Channel(const Channel& other);
  Channel& operator=(const Channel& other);

 public:
  // Orthodox Cannonical Form
  ~Channel();
  Channel(const std::string& name, Client* client);

  // Getters
  const std::string& getName() const;
  const std::string& getTopic() const;
  const std::string& getKey() const;
  int getUserLimit() const;
  bool getIsInviteOnly() const;
  bool getIsTopicRestricted() const;

  const std::set<Client*>& getMember() const;
  const std::set<Client*>& getChanop() const;
  const std::set<Client*>& getInvited() const;
  bool isMember(Client* client) const;
  bool isChanop(Client* client) const;
  bool isInvited(Client* client) const;

  // Setters
  bool setTopic(const std::string& topic);
  bool setKey(const std::string& password);
  bool setUserLimit(int limit);
  bool setInviteOnly(bool isInviteOnly);
  bool setTopicRestricted(bool isTopicRestricted);

  bool addMember(Client* client);
  bool addChanop(Client* client);
  bool addInvited(Client* client);
  bool removeMember(Client* client);
  bool removeChanop(Client* client);
  bool removeInvited(Client* client);

  static const int kMaxChannelNameSize = 50;  // RFC 2811
};

#endif
