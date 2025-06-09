#include <gtest/gtest.h>

#include "TestDataGenerator.hpp"

// 正常
TEST(CommandUser, nomal) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  clients[10] = new Client(10);
  server.addClient(clients[10]);

  std::string msgStr =
      "user username hostname servername :real name with spaces";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "username");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "real name with spaces");
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
}

// 引数なし
TEST(CommandUser, no_arg) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  clients[10] = new Client(10);
  server.addClient(clients[10]);

  std::string msgStr = "USER";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "");
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 461 * USER :Not enough parameters\r\n");
}

// 引数1
TEST(CommandUser, arg1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  clients[10] = new Client(10);
  server.addClient(clients[10]);

  std::string msgStr = "USER username";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "");
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 461 * USER :Not enough parameters\r\n");
}

// 引数2
TEST(CommandUser, arg2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  clients[10] = new Client(10);
  server.addClient(clients[10]);

  std::string msgStr = "USER username hostname";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "");
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 461 * USER :Not enough parameters\r\n");
}

// 引数3
TEST(CommandUser, arg3) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  clients[10] = new Client(10);
  server.addClient(clients[10]);

  std::string msgStr = "USER username hostname servername";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "");
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 461 * USER :Not enough parameters\r\n");
}

// 引数5
TEST(CommandUser, arg5) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  clients[10] = new Client(10);
  server.addClient(clients[10]);

  std::string msgStr = "USER username hostname servername realname extra";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "");
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 461 * USER :Not enough parameters\r\n");
}

// 既にログイン済み
TEST(CommandUser, registered) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "USER username hostname servername realname ";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "user1");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "Real Name 1");
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 462 nick1 :You may not reregister\r\n");
}

// ログイン前2回実行(ngircdではログイン前に2回実行するとエラーになる。ここまで寄せる必要ない気もするけど)
TEST(CommandUser, two_times) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);

  clients[10] = new Client(10);
  server.addClient(clients[10]);

  std::string msgStr1 = "USER username1 hostname servername realname1";
  std::string msgStr2 = "USER username2 hostname servername realname2";

  IRCMessage msg1(clients[10], msgStr1);
  requestHandler.handleCommand(msg1);
  IRCMessage msg2(clients[10], msgStr2);
  requestHandler.handleCommand(msg2);

  EXPECT_EQ(server.getClients().at(10)->getUserName(), "username1");
  EXPECT_EQ(server.getClients().at(10)->getRealName(), "realname1");
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 451 * :You have not registered\r\n");
}
