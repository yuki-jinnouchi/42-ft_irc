#pragma once
#ifndef __COMMAND_NICK_HPP__
#define __COMMAND_NICK_HPP__

#include "ACommand.hpp"

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
};

#endif  // __COMMAND_NICK_HPP__

/*
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
