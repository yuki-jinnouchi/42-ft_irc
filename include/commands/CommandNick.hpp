#pragma once
#ifndef __COMMAND_NICK_HPP__
#define __COMMAND_NICK_HPP__

#include "ACommand.hpp"

/**
  @brief IRC command "NICK" handler.
*/

class CommandNick : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandNick(IRCServer* server);
  ~CommandNick();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandNick();
  CommandNick(const CommandNick& other);
  CommandNick& operator=(const CommandNick& other);

  bool validateNickName(IRCMessage& msg, IRCMessage& reply);
  void announceNickChange(IRCMessage& reply);
};

#endif  // __COMMAND_NICK_HPP__

/*
https://www.rfc-editor.org/rfc/rfc1459#section-4.1.2
4.1.2 Nick message

      Command: NICK
   Parameters: <nickname> [ <hopcount> ]

   NICK message is used to give user a nickname or change the previous
   one.  The <hopcount> parameter is only used by servers to indicate
   how far away a nick is from its home server.  A local connection has
   a hopcount of 0.  If supplied by a client, it must be ignored.

   If a NICK message arrives at a server which already knows about an
   identical nickname for another client, a nickname collision occurs.
   As a result of a nickname collision, all instances of the nickname
   are removed from the server's database, and a KILL command is issued
   to remove the nickname from all other server's database. If the NICK
   message causing the collision was a nickname change, then the
   original (old) nick must be removed as well.

   If the server recieves an identical NICK from a client which is
   directly connected, it may issue an ERR_NICKCOLLISION to the local
   client, drop the NICK command, and not generate any kills.

   Numeric Replies:

           ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
           ERR_NICKNAMEINUSE               ERR_NICKCOLLISION

   Example:

   NICK Wiz                        ; Introducing new nick "Wiz".

   :WiZ NICK Kilroy                ; WiZ changed his nickname to Kilroy.


// ニックネームが長過ぎる場合 o
NICK a234567890
:irc.example.net 432 nick1 a234567890 :Nickname too long, max. 9 characters

// 自分自身のニックネームの場合、何も返さない
NICK nick1

// 引数が足りない場合 o
NICK
:irc.example.net 461 nick1 NICK :Syntax error

// すでにニックネームが存在する場合 o
NICK nick2
:irc.example.net 433 nick1 nick2 :Nickname already in use

// 不正な文字列の場合
NICK 123456789
:irc.example.net 432 nick3 123456789 :Erroneous nickname
NICK #ch
:irc.example.net 432 a23456789 #ch :Erroneous nickname

// ニックネームの変更に成功した場合
NICK a23456789
:nick1!~a@localhost NICK :a23456789
NICK nick1
:NICK!~a@localhost NICK :nick1
// 参加しているチャンネルの他のユーザーにも通知される
// 複数チャンネルに参加していても、通知は1通
:nick1!~a@localhost NICK :nick3
*/
