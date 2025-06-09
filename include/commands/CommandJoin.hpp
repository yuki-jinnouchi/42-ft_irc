#pragma once
#ifndef __COMMAND_JOIN_HPP__
#define __COMMAND_JOIN_HPP__

#include "ACommand.hpp"

/**
 * @brief IRC command "JOIN" handler.
 * @note
    Command: JOIN
    Parameters: <channel>{,<channel>} [<key>{,<key>}]

    The JOIN command is used by client to start listening a specific
    channel. Whether or not a client is allowed to join a channel is
    checked only by the server the client is connected to; all other
    servers automatically add the user to the channel when it is received
    from other servers.  The conditions which affect this are as follows:

            1.  the user must be invited if the channel is invite-only;
            2.  the user's nick/username/hostname must not match any
                active bans;
            3.  the correct key (password) must be given if it is set.

    These are discussed in more detail under the MODE command (see
    section 4.2.3 for more details).

    Once a user has joined a channel, they receive notice about all
    commands their server receives which affect the channel.  This
    includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.  The
    JOIN command needs to be broadcast to all servers so that each server
    knows where to find the users who are on the channel.  This allows
    optimal delivery of PRIVMSG/NOTICE messages to the channel.

    If a JOIN is successful, the user is then sent the channel's topic
    (using RPL_TOPIC) and the list of users who are on the channel (using
    RPL_NAMREPLY), which must include the user joining.

    Numeric Replies:
            ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
            ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
            ERR_CHANNELISFULL               ERR_BADCHANMASK
            ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
            RPL_TOPIC
 * @cite https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.1
 */

class CommandJoin : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandJoin(IRCServer* server);
  ~CommandJoin();

  // Member functions
  void execute(IRCMessage& msg);

  private:
  CommandJoin();
  CommandJoin(const CommandJoin& other);
  CommandJoin& operator=(const CommandJoin& other);

  // Member functions
  bool validJoin(IRCMessage& msg);
  void joinOneChannel(IRCMessage& msg, std::string channelName, std::string key);
  void addClientToNewChannel(IRCMessage& msg, std::string channelName, std::string key);
  void sendResponceToFrom(IRCMessage& msg);
  void sendResponseToChannel(IRCMessage& msg);

};

#endif  // __COMMAND_JOIN_HPP__
