#pragma once
#ifndef __COMMAND_PRIV_MSG_HPP__
#define __COMMAND_PRIV_MSG_HPP__

#include "ACommand.hpp"

/**
  @brief IRC command "PRIVMSG" handler.
*/

class CommandPrivMsg : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandPrivMsg(IRCServer* server);
  ~CommandPrivMsg();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandPrivMsg();
  CommandPrivMsg(const CommandPrivMsg& other);
  CommandPrivMsg& operator=(const CommandPrivMsg& other);

  bool validatePrivMsg(IRCMessage& msg, IRCMessage& reply);
  void sendPrivMsg(std::string& to_str, IRCMessage& reply);
};

#endif  // __COMMAND_PRIV_MSG_HPP__

/*
未ログインの場合
PRIVMSG nick1 hello
(ニックネームなし)
:irc.example.net 451 * :Connection not registered
(ニックネームあり)
:irc.example.net 451 nick1 :Connection not registered

引数不足
PRIVMSG
:irc.example.net 411 nick2 :No recipient given (PRIVMSG)

引数不足（存在しないニックネーム/チャンネル）
PRIVMSG abc
:irc.example.net 412 nick2 :No text to send

引数不足
PRIVMSG nick1
:irc.example.net 412 nick2 :No text to send

引数過剰
PRIVMSG nick1 a b
:irc.example.net 461 nick2 PRIVMSG :Syntax error

存在しないニックネーム/チャンネルの場合
PRIVMSG #ch hello
:irc.example.net 401 nick2 #ch :No such nick or channel name

ニックネームは存在するが、まだログインしていない場合
PRIVMSG nick1 hello
:irc.example.net 401 nick2 nick1 :No such nick or channel name

通常
PRIVMSG nick1 hello
(相手に送信されるメッセージ)
:nick2!~e@localhost PRIVMSG nick1 :hello

通常(自分自身に送信)
PRIVMSG nick1 hello
:nick1!~user1@localhost PRIVMSG nick1 :hello

通常（:だけの場合）
PRIVMSG nick1 :
(相手に送信されるメッセージ)
:nick2!~f@localhost PRIVMSG nick1 :

通常（日本語、絵文字、スペース）
PRIVMSG nick1 :寿司🍣 酒池肉林🍺🍖 Hello World!
:nick2!~f@localhost PRIVMSG nick1 :寿司🍣酒池肉林🍺🍖

通常（長いメッセージ）
PRIVMSG nick2
12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234
:nick1!~user1@localhost PRIVMSG nick2
:1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456[CUT]

// カンマ区切りの複数のニックネーム
PRIVMSG nick2,nick3 hello
:nick1!~a@localhost PRIVMSG nick2 :hello
:nick1!~a@localhost PRIVMSG nick3 :hello

// カンマ区切りの複数のニックネーム（存在しないニックネームが含まれる）
PRIVMSG nick2,xxx hello
:irc.example.net 401 nick1 xxx :No such nick or channel name
:nick1!~a@localhost PRIVMSG nick2 :hello

// チャンネルに所属していなくても、メッセージは送信可能（プライベートでも）
// カンマ区切りチャンネル、ニックネーム混合
PRIVMSG nick2,#ch1 hello
:nick1!~a@localhost PRIVMSG nick2 :hello
:nick1!~a@localhost PRIVMSG #ch1 :hello

// 512文字以上のメッセージ(BANされる)
PRIVMSG nick1
12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
:irc.example.net NOTICE nick2 :Connection statistics: client 1.3 kb, server 2.6
kb.
ERROR :Request too long

// チャンネルが外部送信不可の場合はエラー
mode #ch1 +n //  no messages to channel from clients on the outside;
privmsg #ch1 hello
:irc.example.net 404 nick1 #ch1 :Cannot send to channel

*/
