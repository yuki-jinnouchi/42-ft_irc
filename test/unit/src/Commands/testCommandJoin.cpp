#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"

static void makeUserData(IRCServer &server, std::map<int, Client *> &clients) {
  clients[10] = new Client(10);
  clients[10]->setNickName("nick1");
  clients[10]->setUserName("user1");
  clients[10]->setIsRegistered(true);

  clients[11] = new Client(11);
  clients[11]->setNickName("nick2");
  clients[11]->setUserName("user2");
  clients[11]->setIsRegistered(true);

  // clients[12] = new Client(12);
  // clients[12]->setNickName("nick3");
  // clients[12]->setUserName("user3");

  server.addClient(clients[10]);
  server.addClient(clients[11]);
  // server.addClient(clients[12]);
}

// 正常（新規JOIN - #）
TEST(CommandJoin, normalJoin) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply =
      ":nick1!~user1@localhost JOIN #channel"
      "\r\n"
      ":irc.example.net 332 nick1 #channel :"
      "\r\n"
      ":irc.example.net 353 nick1 #channel :nick1"
      "\r\n"
      ":irc.example.net 366 nick1 #channel :End of /NAMES list";
  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
  EXPECT_TRUE(server.getChannel(channelName)->isMember(clients[10]));
  EXPECT_TRUE(server.getChannel(channelName)->isChanop(clients[10]));
}

// 正常 (新規JOIN - &)
TEST(CommandJoin, withAndNewChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "&channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply =
      ":nick1!~user1@localhost JOIN &channel"
      "\r\n"
      ":irc.example.net 332 nick1 &channel :"
      "\r\n"
      ":irc.example.net 353 nick1 &channel :nick1"
      "\r\n"
      ":irc.example.net 366 nick1 &channel :End of /NAMES list";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 正常 (チャンネルが存在する場合)
TEST(CommandJoin, withExistingChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply01 =
      ":nick1!~user1@localhost JOIN #channel"
      "\r\n"
      ":irc.example.net 332 nick1 #channel :"
      "\r\n"
      ":irc.example.net 353 nick1 #channel :nick1"
      "\r\n"
      ":irc.example.net 366 nick1 #channel :End of /NAMES list";
  std::string expected_reply02 =
      ":nick2!~user2@localhost JOIN #channel"
      "\r\n"
      ":irc.example.net 332 nick2 #channel :"
      "\r\n"
      ":irc.example.net 353 nick2 #channel :nick1 nick2"
      "\r\n"
      ":irc.example.net 366 nick2 #channel :End of /NAMES list";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply01 + "\r\n");

  clients[10]->consumeSendingMsg(100000);  // Clear sending message
  clients[11]->consumeSendingMsg(100000);  // Clear sending message

  IRCMessage msg02(clients[11], msgStr);
  requestHandler.handleCommand(msg02);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[11]), true);
  EXPECT_EQ(clients[11]->getSendingMsg(), expected_reply02 + "\r\n");
}

// 正常 (すでに参加しているチャンネルに再度参加する場合)
TEST(CommandJoin, rejoinChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  std::string expected_reply01 =
      ":nick1!~user1@localhost JOIN #channel"
      "\r\n"
      ":irc.example.net 332 nick1 #channel :"
      "\r\n"
      ":irc.example.net 353 nick1 #channel :nick1"
      "\r\n"
      ":irc.example.net 366 nick1 #channel :End of /NAMES list";

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply01 + "\r\n");
  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);

  clients[10]->consumeSendingMsg(100000);  // Clear sending message

  IRCMessage msg02(clients[10], msgStr);
  requestHandler.handleCommand(msg02);

  std::string expected_reply = "";
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply);
  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);
}

// 異常 (パラメーターがない場合)
TEST(CommandJoin, noParameters) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN";

  std::string expected_reply =
      ":irc.example.net 461 nick1 JOIN :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getChannel(channelName), nullptr);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 異常 (パラメーターが多すぎる場合)
TEST(CommandJoin, tooManyParameters) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName + " extra_a extra_b";
  std::string expected_reply =
      ":irc.example.net 461 nick1 JOIN :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getChannel(channelName), nullptr);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
  EXPECT_FALSE(server.ifChannleExists(channelName));
}

// 異常 (チャンネル名が不正な場合)
TEST(CommandJoin, abnormalChannelName) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected = "#channel";
  std::string expected_reply =
      ":irc.example.net 403 nick1 channel :No such channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getChannel(channelName), nullptr);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 異常 (チャンネルが招待制限されている場合)
TEST(CommandJoin, inviteOnlyChannel_notInvited) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply =
      ":irc.example.net 473 nick1 #channel :Cannot join channel (+i)";

  server.addChannel(channelName, clients[11]);
  server.getChannel(channelName)->setInviteOnly(true);

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), false);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 正常 (チャンネルが招待制限されており、招待されている場合)
TEST(CommandJoin, inviteOnlyChannel_invited) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply =
      ":nick1!~user1@localhost JOIN #channel"
      "\r\n"
      ":irc.example.net 332 nick1 #channel :"
      "\r\n"
      ":irc.example.net 353 nick1 #channel :nick1 nick2"
      "\r\n"
      ":irc.example.net 366 nick1 #channel :End of /NAMES list";

  server.addChannel(channelName, clients[11]);
  Channel *channel = server.getChannel(channelName);
  channel->setInviteOnly(true);
  channel->addInvited(clients[10]);
  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_TRUE(server.getChannel(channelName)->isMember(clients[10]));
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
  EXPECT_EQ(server.getChannel(channelName)->getMember().size(), 2);
}

// 異常 (チャンネルが満員の場合)
TEST(CommandJoin, fullChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply =
      ":irc.example.net 471 nick1 #channel :Cannot join channel (+l)";

  IRCMessage msg01(clients[11], msgStr);
  requestHandler.handleCommand(msg01);

  server.getChannel(channelName)->setUserLimit(1);
  IRCMessage msg02(clients[10], msgStr);
  clients[10]->consumeSendingMsg(100000);  // Clear sending message
  requestHandler.handleCommand(msg02);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), false);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
  EXPECT_EQ(server.getChannel(channelName)->getMember().size(), 1);
  EXPECT_EQ(server.getChannel(channelName)->getUserLimit(), 1);
}

// 異常 (チャンネルが招待制で、再度参加しようとする場合)
TEST(CommandJoin, inviteOnlyChannel_rejoin) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply01 =
      ":nick1!~user1@localhost JOIN #channel"
      "\r\n"
      ":irc.example.net 332 nick1 #channel :"
      "\r\n"
      ":irc.example.net 353 nick1 #channel :nick1 nick2"
      "\r\n"
      ":irc.example.net 366 nick1 #channel :End of /NAMES list";

  server.addChannel(channelName, clients[11]);
  Channel *channel = server.getChannel(channelName);
  channel->setInviteOnly(true);
  channel->addInvited(clients[10]);

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_TRUE(server.getChannel(channelName)->isMember(clients[10]));
  EXPECT_FALSE(server.getChannel(channelName)->isInvited(clients[10]));
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply01 + "\r\n");
  EXPECT_EQ(server.getChannel(channelName)->getMember().size(), 2);

  clients[10]->consumeSendingMsg(100000);  // Clear sending message
  channel->removeMember(clients[10]);      // Remove client from channel
  std::string expected_reply02 =
      ":irc.example.net 473 nick1 #channel :Cannot join channel (+i)";

  requestHandler.handleCommand(msg);

  EXPECT_FALSE(server.getChannel(channelName)->isMember(clients[10]));
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply02 + "\r\n");
  EXPECT_EQ(server.getChannel(channelName)->getMember().size(), 1);
}
