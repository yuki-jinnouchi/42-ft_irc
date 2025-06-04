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
  // server.addClient(clients[11]);
  // server.addClient(clients[12]);
}

// 正常 (チャンネルが存在しない場合)
TEST(CommandJoin, withHashNewChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply = ":nick1!~user1@localhost JOIN #channel";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 正常 (チャンネルが存在しない場合)
TEST(CommandJoin, withAndNewChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "&channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply = ":nick1!~user1@localhost JOIN &channel";

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
  std::string expected_reply01 = ":nick1!~user1@localhost JOIN #channel";
  std::string expected_reply02 = ":nick2!~user2@localhost JOIN #channel";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);
  IRCMessage msg02(clients[11], msgStr);
  requestHandler.handleCommand(msg02);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply01 + "\r\n");
  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[11]), true);
  EXPECT_EQ(clients[11]->getSendingMsg(), expected_reply02 + "\r\n");
}

// 異常 (すでに参加しているチャンネルに再度参加する場合)
TEST(CommandJoin, rejoinChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  server.addChannel("#channel", clients[10]);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN " + channelName;
  std::string expected_reply = "";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(server.getChannel(channelName)->isMember(clients[10]), true);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply);
}

// 異常 (パラメーターがない場合)
TEST(CommandJoin, noParameters) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string channelName = "#channel";
  std::string msgStr = "JOIN";

  std::string expected_reply = ":irc.example.net 461 nick1 JOIN :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getChannel(channelName), nullptr);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
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
  std::string expected_reply = ":irc.example.net 403 nick1 channel :No such channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getChannel(channelName), nullptr);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}
