#pragma once
#ifndef __COMMAND_TOPIC_HPP__
#define __COMMAND_TOPIC_HPP__

#include "ACommand.hpp"

/**
 * @brief IRC command "TOPIC" handler.
 * @details This class handles the "TOPIC" command in IRC, which is used to
 *          change or view the topic of a channel.
 * @note
    Command: TOPIC
    Parameters: <channel> [<topic>]

    The TOPIC message is used to change or view the topic of a channel.
    The topic for channel <channel> is returned if there is no <topic>
    given.  If the <topic> parameter is present, the topic for that
    channel will be changed, if the channel modes permit this action.

    Numeric Replies:
            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
            RPL_NOTOPIC                     RPL_TOPIC
            ERR_CHANOPRIVSNEEDED
 * @cite https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.4
 */

class CommandTopic : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandTopic(IRCServer* server);
  ~CommandTopic();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandTopic();
  CommandTopic(const CommandTopic& other);
  CommandTopic& operator=(const CommandTopic& other);

  bool validateTopic(IRCMessage& msg, IRCMessage& reply);
};

#endif  // __COMMAND_TOPIC_HPP__
