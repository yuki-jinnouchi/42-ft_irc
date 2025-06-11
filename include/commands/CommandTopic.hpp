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

  bool validateTopic(IRCMessage& msg);
  Channel* getAndValidateChannel(IRCMessage& msg);
};

#endif  // __COMMAND_TOPIC_HPP__

/*
// チャンネルに参加している人は誰でもトピックを取得・設定できる
// 管理者権限不要（Tモード以外）

// ログインしていない場合
// 引数不足、過剰

// トピックがない場合
topic #ch1
:irc.example.net 331 nick1 #ch1 :No topic is set

// トピックがある場合
topic #ch1
:irc.example.net 332 nick1 #ch1 :this is a topic
:irc.example.net 333 nick1 #ch1 nick1 1749534889

// トピックを設定する
topic #ch1 :this is a topic
:nick1!~a@localhost TOPIC #ch1 :this is a topic

// チャンネルがない場合
topic #ch2
:irc.example.net 403 nick1 #ch2 :No such channel

// チャンネルに参加していない場合、取得・設定できない
topic #ch3 :no ope
:irc.example.net 442 nick1 #ch3 :You are not on that channel
topic #ch3
:irc.example.net 442 nick1 #ch3 :You are not on that channel

// チャンネルがtモード、かつ権限がない場合
topic #ch1 :a bb cc
:irc.example.net 482 nick2 #ch1 :You are not channel operator
*/
