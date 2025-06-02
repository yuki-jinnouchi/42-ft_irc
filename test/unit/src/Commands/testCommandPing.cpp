#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"

// 通常
TEST(CommandPing, nomal) {
  IRCServer server("6677", "pass123");

  std::map<int, Client*> clients;
  clients[10] = new Client(10);
  clients[10]->setNickName("nick1");

  server.addClient(clients[10]);

  std::string msgStr = "PING 12345678901234567890";
  std::string expected =
      ":irc.example.net PONG irc.example.net :12345678901234567890";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}

// 引数なし
TEST(CommandPing, no_arg) {
  IRCServer server("6677", "pass123");

  std::map<int, Client*> clients;
  clients[10] = new Client(10);
  clients[10]->setNickName("nick1");

  server.addClient(clients[10]);

  std::string msgStr = "PING";
  std::string expected = ":irc.example.net 409 nick1 :No origin specified";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}

// 返信が512文字を超える場合
TEST(CommandPing, responce_over_512) {
  IRCServer server("6677", "pass123");

  std::map<int, Client*> clients;
  clients[10] = new Client(10);
  clients[10]->setNickName("nick1");

  server.addClient(clients[10]);

  std::string msgStr =
      "PING "
      "123456789012345678901234567890123456789012345678901234567890123456789012"
      "345678901234567890123456789012345678901234567890123456789012345678901234"
      "567890123456789012345678901234567890123456789012345678901234567890123456"
      "789012345678901234567890123456789012345678901234567890123456789012345678"
      "901234567890123456789012345678901234567890123456789012345678901234567890"
      "123456789012345678901234567890123456789012345678901234567890123456789012"
      "3456789012345678901234567890123456789012345678901234567890123456789";
  std::string expected =
      ":irc.example.net PONG irc.example.net "
      ":12345678901234567890123456789012345678901234567890123456789012345678901"
      "234567890123456789012345678901234567890123456789012345678901234567890123"
      "456789012345678901234567890123456789012345678901234567890123456789012345"
      "678901234567890123456789012345678901234567890123456789012345678901234567"
      "890123456789012345678901234567890123456789012345678901234567890123456789"
      "012345678901234567890123456789012345678901234567890123456789012345678901"
      "23456789012345678901234567890123456[CUT]";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}
