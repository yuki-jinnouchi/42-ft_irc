#pragma once
#ifndef __COMMAND_KICK_HPP__
#define __COMMAND_KICK_HPP__

#include "ACommand.hpp"

class CommandKick : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandKick(IRCServer* server);
  ~CommandKick();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandKick();
  CommandKick(const CommandKick& other);
  CommandKick& operator=(const CommandKick& other);

  bool validateKick(IRCMessage& msg);
  Channel* getAndValidateChannel(IRCMessage& msg, const std::string& ch_str);
  Client* getAndValidateClient(IRCMessage& msg, Channel* channel,
                               const std::string& nick);

  void kickMembers(IRCMessage& msg, Channel* channel,
                   const std::vector<std::string> nicks);
};

#endif  // __COMMAND_KICK_HPP__

/*
https://www.rfc-editor.org/rfc/rfc1459#section-4.2.8
4.2.8 Kick command

      Command: KICK
   Parameters: <channel> <user> [<comment>]

   The KICK command can be  used  to  forcibly  remove  a  user  from  a
   channel.   It  'kicks  them  out'  of the channel (forced PART).
   Only a channel operator may kick another user out of a  channel.
   Each  server that  receives  a KICK message checks that it is valid
   (ie the sender is actually a  channel  operator)  before  removing
   the  victim  from  the channel.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
           ERR_NOTONCHANNEL

   Examples:

KICK &Melbourne Matthew         ; Kick Matthew from &Melbourne

KICK #Finnish John :Speaking English
                                ; Kick John from #Finnish using
                                "Speaking English" as the reason
                                (comment).

:WiZ KICK #Finnish John         ; KICK message from WiZ to remove John
                                from channel #Finnish

NOTE:
     It is possible to extend the KICK command parameters to the
following:

<channel>{,<channel>} <user>{,<user>} [<comment>]

// 通常（コメントなし）
KICK #ch nick2
:nick1!~a@localhost KICK #ch nick2 :nick1

// 通常(コメントあり)
KICK #ch1 nick2 bye
:nick1!~a@localhost KICK #ch1 nick2 :bye

// ログインしていない
:irc.example.net 451 * :Connection not registered

// 引数が足りない
// 引数が多い
KICK #ch1 nick2 a b
:irc.example.net 461 nick1 KICK :Syntax error

// チャンネルに所属していない
KICK #ch1 nick2
:irc.example.net 442 nick1 #ch1 :You are not on that channel

//　権限がない（チャンネルのオペレーターではない）
KICK #ch1 nick1
:irc.example.net 482 nick2 #ch1 :Your privileges are too low

// チャンネルがない
KICK #chx nick3 bye
:irc.example.net 403 nick1 #chx :No such channel
// ニックネームがない
KICK #ch1 nickx bye
:irc.example.net 401 nick1 nickx :No such nick or channel name

// ユーザーがチャンネルにいない
KICK #ch2 nick3 bye
:irc.example.net 441 nick1 nick3 #ch2 :They aren't on that channel

// 自分自身はKICKできる？ 可能
// operatorをKICKできる？　可能
*/
