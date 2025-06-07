#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"
#include "TestDataGenerator.hpp"

// 正常（１チャンネルから退出）
TEST(CommandPart, normalPart) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string channelName = "#ch2";
  std::string msgStr = "PART " + channelName;
  std::string expected_reply = ":nick1!~user1@localhost PART #ch2";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_FALSE(server.getChannel(channelName)->isMember(clients[10]));
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 正常 (複数チャンネルから退出)
TEST(CommandPart, normalPartMultipleChannels) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string channelNames = "#ch1,#ch2";

  std::string msgStr = "PART " + channelNames;
  std::string expected_reply1 =
      ":nick1!~user1@localhost PART #ch1"
      "\r\n"
      ":nick1!~user1@localhost PART #ch2";

  std::string expected_reply2 = ":nick1!~user1@localhost PART #ch2";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_FALSE(server.getChannel("#ch1")->isMember(clients[10]));
  EXPECT_FALSE(server.getChannel("#ch2")->isMember(clients[10]));

  std::string sendingMsg = clients[10]->getSendingMsg();
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply1 + "\r\n");
  EXPECT_EQ(clients[11]->getSendingMsg(), expected_reply2 + "\r\n");
}

// // 異常 (パラメーターがない場合)
TEST(CommandPart, noParameters) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PART";
  std::string expected_reply =
      ":irc.example.net 461 nick1 PART :Not enough parameters";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 異常（パラメーターが多すぎる場合）
TEST(CommandPart, tooManyParameters) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string channelName = "#ch1";
  std::string msgStr = "PART #ch1 extra param";
  std::string expected_reply =
      ":irc.example.net 461 nick1 PART :Not enough parameters";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_TRUE(server.getChannel(channelName)->isMember(clients[10]));
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 異常 (チャンネルが存在しない場合)
TEST(CommandPart, channelNotExists) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PART #noexist";
  std::string expected_reply =
      ":irc.example.net 403 nick1 #noexist :No such channel";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// // 正常 (チャンネルオペレーターが退出する場合)
TEST(CommandPart, operatorPart) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string channelName = "#ch1";
  std::string msgStr = "PART " + channelName;
  std::string expected_reply = ":nick1!~user1@localhost PART #ch1";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_FALSE(server.getChannel(channelName)->isMember(clients[10]));
  EXPECT_FALSE(server.getChannel(channelName)->isChanop(clients[10]));
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 異常 (チャンネルに参加していない場合)
TEST(CommandPart, notOnChannel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string channelName = "#ch4";
  std::string msgStr = "PART " + channelName;
  std::string expected_reply =
      ":irc.example.net 442 nick1 #ch4 :You're not on that channel";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 正常 (最後のメンバーが退出してチャンネルが削除される場合)
TEST(CommandPart, lastMemberPartChannelDestroyed) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string channelName = "#ch1";

  Channel *channel = server.getChannel(channelName);

  IRCMessage msg01(clients[13], "PART " + channelName);
  requestHandler.handleCommand(msg01);
  clients[10]->consumeSendingMsg(
      100000);  // クライアントの送信メッセージをクリア

  std::string msgStr = "PART " + channelName;
  std::string expected_reply = ":nick1!~user1@localhost PART #ch1";

  IRCMessage msg02(clients[10], msgStr);
  requestHandler.handleCommand(msg02);

  EXPECT_EQ(server.getChannel(channelName), nullptr);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}
