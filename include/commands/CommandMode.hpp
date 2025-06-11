#pragma once
#ifndef __COMMAND_MODE_HPP__
#define __COMMAND_MODE_HPP__

#include "ACommand.hpp"

/**
 * @brief IRC command "MODE" handler.
 * @note
    Command: MODE
    Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
                [<ban mask>]

    The MODE command is provided so that channel operators may change the
    characteristics of `their' channel.  It is also required that servers
    be able to change channel modes so that channel operators may be
    created.

    The various modes available for channels are as follows:

            o - give/take channel operator privileges;
            p - private channel flag;
            s - secret channel flag;
            i - invite-only channel flag;
            t - topic settable by channel operator only flag;
            n - no messages to channel from clients on the outside;
            m - moderated channel;
            l - set the user limit to channel;
            b - set a ban mask to keep users out;
            v - give/take the ability to speak on a moderated channel;
            k - set a channel key (password).

    When using the 'o' and 'b' options, a restriction on a total of three
    per mode command has been imposed.  That is, any combination of 'o'
    and

    (...User Modes...)

       Replies:
           ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_KEYSET
           RPL_BANLIST                     RPL_ENDOFBANLIST
           ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

           ERR_USERSDONTMATCH              RPL_UMODEIS
           ERR_UMODEUNKNOWNFLAG

 * @cite https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.3
 *
 * @note in en.subject.pdf
    - MODE - Change the channel’s mode:
      - i: Set/remove Invite-only channel
      - t: Set/remove the restrictions of the TOPIC command to channel operators
      - k: Set/remove the channel key (password)
      - o: Give/take channel operator privilege
      - l: Set/remove the user limit to channel
 */

class CommandMode : public ACommand {
  private:
  CommandMode();
  CommandMode(const CommandMode& other);
  CommandMode& operator=(const CommandMode& other);

  // Member functions
  bool validateMode(IRCMessage& msg);
   bool validClient(IRCMessage& msg, Client* client);
  bool plusI(Channel* channel, Client* from);
  bool minusI(Channel* channel, Client* from);
  bool plusT(Channel* channel, Client* from);
  bool minusT(Channel* channel, Client* from);
  bool plusK(Channel* channel, Client* from, const std::string& key);
  bool minusK(Channel* channel, Client* from);
  bool plusO(Channel* channel, Client* from, Client* client);
  bool minusO(Channel* channel, Client* from, Client* client);
  bool plusL(Channel* channel, Client* from, std::string limit);
  bool minusL(Channel* channel, Client* from);

  public:
   // Orthodox Canonical Form
   CommandMode(IRCServer* server);
   ~CommandMode();

   // Member functions
   void execute(IRCMessage& msg);

};

#endif  // __COMMAND_MODE_HPP__
