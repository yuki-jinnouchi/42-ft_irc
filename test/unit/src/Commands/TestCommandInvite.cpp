#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"
#include "TestDataGenerator.hpp"

// 正常（招待するユーザーが存在し、チャンネルに参加している）
TEST(CommandInvite, normal_invite_existing_user) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick2 #ch1";
  std::string expected_10 = ":irc.example.net 341 nick1 nick2 #ch1";
  std::string expected_11 = ":nick1!~user1@localhost INVITE nick2 #ch1";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_10 + "\r\n");
  EXPECT_EQ(clients[11]->getSendingMsg(), expected_11 + "\r\n");
  EXPECT_TRUE(server.getChannel("#ch1")->isInvited(clients[11]));
}

// 異常（パラメータが不足している - 空のコマンド）
TEST(CommandInvite, invite_no_parameters) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE";
  std::string expected =
      ":irc.example.net 461 nick1 INVITE :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}

// 異常（パラメータが不足している - ユーザー名のみ）
TEST(CommandInvite, invite_missing_channel_parameter) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick2";
  std::string expected =
      ":irc.example.net 461 nick1 INVITE :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}

// 異常（パラメータが多すぎる）
TEST(CommandInvite, invite_too_many_parameters) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick2 #ch1 extra_param";
  std::string expected =
      ":irc.example.net 461 nick1 INVITE :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  EXPECT_TRUE(server.getChannel("#ch1")->getInvited().empty());
}

// 異常（招待するユーザーが存在しない）
TEST(CommandInvite, invite_noexist_user) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE noexist #ch1";
  std::string expected =
      ":irc.example.net 401 nick1 noexist :No such nick/channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  EXPECT_TRUE(server.getChannel("#ch1")->getInvited().empty());
}

// 異常（チャンネルが存在しない）
TEST(CommandInvite, invite_to_noexist_channel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick2 #noexist";
  std::string expected = ":irc.example.net 403 nick1 #noexist :No such channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  EXPECT_TRUE(server.getChannel("#noexist") == nullptr);
}

// 異常（招待者がそもそもチャンネルに参加していない）
TEST(CommandInvite, invite_not_on_channel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick2 #ch5";
  std::string expected =
      ":irc.example.net 442 nick1 #ch5 :You're not on that channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}

// 異常（招待されるユーザーが既にチャンネルに参加している）
TEST(CommandInvite, invite_user_already_on_channel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick2 #ch2";
  std::string expected =
      ":irc.example.net 443 nick1 nick2 #ch2 :is already on channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}

// 異常（招待するためのオペレーター権限がない）
TEST(CommandInvite, invite_without_operator_privileges) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick3 #ch2";
  std::string expected =
      ":irc.example.net 482 nick2 #ch2 :You're not channel operator";

  IRCMessage msg(clients[11], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
}

// 異常（同じユーザーを複数回招待した場合、無視）
TEST(CommandInvite, invite_same_user_twice) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick2 #ch1";

  IRCMessage msg1(clients[10], msgStr);
  requestHandler.handleCommand(msg1);

  clients[10]->consumeSendingMsg(100000);  // 送信メッセージをクリア
  clients[11]->consumeSendingMsg(100000);  // 送信メッセージをクリア

  std::string expected_10 = "";
  std::string expected_11 = "";

  IRCMessage msg2(clients[10], msgStr);
  requestHandler.handleCommand(msg2);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_10 + "");
  EXPECT_EQ(clients[11]->getSendingMsg(), expected_11 + "");
  EXPECT_TRUE(server.getChannel("#ch1")->isInvited(clients[11]));
}

// 異常（招待するユーザーが未ログイン）
TEST(CommandInvite, not_registered_user) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "INVITE nick5 #ch1";
  std::string expected =
      ":irc.example.net 401 nick1 nick5 :No such nick/channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  EXPECT_TRUE(server.getChannel("#ch1")->getInvited().empty());
}
