#pragma once
#ifndef __COMMAND_PART_HPP__
#define __COMMAND_PART_HPP__

#include "ACommand.hpp"

/**
 * @brief IRC command "PART" handler.
 * @note
    Command: PART
    Parameters: <channel>{,<channel>}
    The PART message causes the client sending the message to be removed
    from the list of active users for all given channels listed in the
    parameter string.

    Numeric Replies:
            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
            ERR_NOTONCHANNEL
 * @cite https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.2
 */

class CommandPart : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandPart(IRCServer* server);
  ~CommandPart();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandPart();
  CommandPart(const CommandPart& other);
  CommandPart& operator=(const CommandPart& other);

  bool validatePart(IRCMessage& msg);
  bool validChannel(IRCMessage& msg, const std::string& channelName);
};

#endif  // __COMMAND_PART_HPP__
