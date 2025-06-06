#include <gtest/gtest.h>

#include "TestDataGenerator.hpp"

// 通常
TEST(CommandNick, nomal) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK a23456789";
  std::string expected = "a23456789";
  std::string expected_reply = ":nick1!~user1@localhost NICK :a23456789";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
  // TODO チャンネルの他のユーザーへの通知をテストする
  EXPECT_EQ(clients[11]->getSendingMsg(), expected_reply + "\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), expected_reply + "\r\n");
  EXPECT_EQ(clients[13]->getSendingMsg(), expected_reply + "\r\n");
  EXPECT_EQ(clients[14]->getSendingMsg(), "");
  EXPECT_EQ(clients[15]->getSendingMsg(), "");
}

// 通常(未ログインの場合、返信しない)
TEST(CommandNick, nomal2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK a23456789";
  std::string expected = "a23456789";

  IRCMessage msg(clients[14], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(14)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
}

// ニックネームが長過ぎる
TEST(CommandNick, too_long) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK a234567890";
  std::string expected = "nick1";
  std::string expected_reply =
      ":irc.example.net 432 nick1 a234567890 :Nickname too long, max. 9 "
      "characters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // ニックネームは変更されない
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");

  // 他のユーザーには通知されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// ニックネームが自分と同じ
TEST(CommandNick, same_nickname) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK nick1";
  std::string expected = "nick1";
  std::string expected_reply = "";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // ニックネームは変更されない
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply);

  // 他のユーザーには通知されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 引数が足りない
TEST(CommandNick, no_args) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK ";
  std::string expected = "nick1";
  std::string expected_reply =
      ":irc.example.net 461 nick1 NICK :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // ニックネームは変更されない
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");

  // 他のユーザーには通知されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// すでに存在するニックネーム
TEST(CommandNick, dup_nickname) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK nick2";
  std::string expected = "nick1";
  std::string expected_reply =
      ":irc.example.net 433 nick1 nick2 :Nickname already in use";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // ニックネームは変更されない
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");

  // 他のユーザーには通知されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// ニックネームが不正な文字列
TEST(CommandNick, invalid_nickname1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK 1nick";
  std::string expected = "nick1";
  std::string expected_reply =
      ":irc.example.net 432 nick1 1nick :Erroneous nickname";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // ニックネームは変更されない
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");

  // 他のユーザーには通知されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// ニックネームが不正な文字列2
TEST(CommandNick, invalid_nickname2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK nick@";
  std::string expected = "nick1";
  std::string expected_reply =
      ":irc.example.net 432 nick1 nick@ :Erroneous nickname";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // ニックネームは変更されない
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");

  // 他のユーザーには通知されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// ニックネームが不正な文字列3
TEST(CommandNick, invalid_nickname3) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "NICK #ch";
  std::string expected = "nick1";
  std::string expected_reply =
      ":irc.example.net 432 nick1 #ch :Erroneous nickname";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // ニックネームは変更されない
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");

  // 他のユーザーには通知されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}
