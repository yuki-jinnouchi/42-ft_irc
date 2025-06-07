#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"
#include "TestDataGenerator.hpp"

// 正常（大文字）
TEST(CommandPart, capitalNull) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "ABCDE";
  std::string expected_reply = ":irc.example.net 421 nick1 ABCDE :Unknown command";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 正常（小文字）
TEST(CommandPart, smallNull) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "abcde";
  std::string expected_reply = ":irc.example.net 421 nick1 abcde :Unknown command";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

// 正常（数字）
TEST(CommandPart, numNull) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "999";
  std::string expected_reply = ":irc.example.net 421 nick1 999 :Unknown command";

  IRCMessage msg01(clients[10], msgStr);
  requestHandler.handleCommand(msg01);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected_reply + "\r\n");
}

