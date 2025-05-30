#pragma once
#ifndef __A_COMMAND_HPP__
#define __A_COMMAND_HPP__

#include <vector>

#include "IRCMessage.hpp"
#include "IRCServer.hpp"

class ACommand {
 public:
  // Orthodox Canonical Form
  ACommand(IRCServer* server);
  virtual ~ACommand();

  virtual void execute(IRCMessage& msg) = 0;
  std::string generateReply(const IRCMessage& orig,
                            const IRCMessage& reply) const;

 protected:
  // Member variables
  IRCServer* server_;
  // std::vector<IRCMessage> replies_; 使わない？

  ACommand(const ACommand& other);
  ACommand& operator=(const ACommand& other);
};

#endif  // __A_COMMAND_HPP__

/*
PING
:irc.example.net 409 nick1 :No origin specified
PING abc
:irc.example.net PONG irc.example.net :abc
PING :abc
:irc.example.net PONG irc.example.net :abc
PING :a b c d
:irc.example.net PONG irc.example.net :a b c d
PING a irc.example.net c d
:irc.example.net PONG irc.example.net :a
PING a localhost c d
:irc.example.net 402 nick1 localhost :No such server

nick
===========================
- ニックネームの変更
:nick3!~a@localhost NICK :nick4

- チャンネルユーザーにニックネームの変更を通知
（複数のチャンネルに所属している場合は、2回送らないように。1回だけ送信）
:hoge!~a@localhost NICK :nick3

- 重複チェック

- 長さチェック
NICK 1234567890
:irc.example.net 432 nick3 1234567890 :Nickname too long, max. 9 characters

- 返信が512を超える場合は後ろがカットされる
:irc.example.net 461 nick3 NICK :Syntax error
NICK
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345
:irc.example.net 432 nick3
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678[CUT]

- 引数が1個以外はシンタックスエラー
NICK a b
:irc.example.net 461 nick3 NICK :Syntax error
NICK
:irc.example.net 461 nick3 NICK :Syntax error

join
===========================
- チャンネルに参加（新規作成）
join #hoge
:nick1!~a@localhost JOIN :#hoge
:irc.example.net 353 nick1 = #hoge :@nick1
:irc.example.net 366 nick1 #hoge :End of NAMES list

- チャンネルに参加（既存）
join #hoge5
:nick5!~a@localhost JOIN :#hoge5
:irc.example.net 353 nick5 = #hoge5 :nick5 @nick1
:irc.example.net 366 nick5 #hoge5 :End of NAMES list
他のユーザーに通知
join #hoge:nick5!~a@localhost JOIN :#hoge5


- 存在しないチャンネル
join hoge
:irc.example.net 403 nick1 hoge :No such channel

*/
