#pragma once
#ifndef __COMMAND_INVITE_HPP__
#define __COMMAND_INVITE_HPP__

#include "ACommand.hpp"

/**
 * @brief IRC command "INVITE" handler.
 * @details This class handles the "INVITE" command in IRC, which is used to invite a user to a channel.
 *
 * @note
    Command: INVITE
    Parameters: <nickname> <channel>

    The INVITE message is used to invite users to a channel.  The
    parameter <nickname> is the nickname of the person to be invited to
    the target channel <channel>.  There is no requirement that the
    channel the target user is being invited to must exist or be a valid
    channel.  To invite a user to a channel which is invite only (MODE
    +i), the client sending the invite must be recognised as being a
    channel operator on the given channel.

    Numeric Replies:
            ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
            ERR_NOTONCHANNEL                ERR_USERONCHANNEL
            ERR_CHANOPRIVSNEEDED
            RPL_INVITING                    RPL_AWAY
    @cite https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.7
*/

class CommandInvite : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandInvite(IRCServer* server);
  ~CommandInvite();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandInvite();
  CommandInvite(const CommandInvite& other);
  CommandInvite& operator=(const CommandInvite& other);

  // Member functions
  bool validateInvite(IRCMessage& msg);
};

#endif  // __COMMAND_INVITE_HPP__
