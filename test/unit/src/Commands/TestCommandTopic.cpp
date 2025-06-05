#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"

static void makeUserData(IRCServer &server, std::map<int, Client *> &clients) {
  clients[10] = new Client(10);
  clients[10]->setNickName("nick1");
  clients[10]->setUserName("user1");
  clients[10]->setIsRegistered(true);
  server.addChannel("#channel1", clients[10]);  // チャンネルに参加
  server.getChannel("#channel1")->setTopic("Initial topic");
}

// 通常
TEST(CommandTopic, nomal1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  RequestHandler requestHandler(&server);

  std::string msgStr = "TOPIC #channel1";
  std::string expected = ":irc.example.net 332 nick1 #channel1 :Initial topic";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
}
