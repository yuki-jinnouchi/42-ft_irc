#include <gtest/gtest.h>
#include "CommandHandler.hpp"
#include "IRCServer.hpp"

TEST(CommandHandler, broadCastRawMsg) {
  IRCServer server("6677", "pass123");

  std::map<int, ClientSession*> clients;
  clients[10] = new ClientSession(10);
  clients[11] = new ClientSession(11);
  clients[12] = new ClientSession(12);
  server.addClient(clients[10]);
  server.addClient(clients[11]);
  server.addClient(clients[12]);

  std::string msgStr = "abcdefg";
  IRCMessage msg(clients[10], msgStr);

  CommandHandler commandHandler(&server);
  commandHandler.broadCastRawMsg(msg);

  const std::map<ClientSession*, std::string> res = msg.getResponses();

  EXPECT_EQ(res.size(), 2);

  // 11, 12にそのままのメッセージが送信されていることを確認
  EXPECT_EQ(res.at(clients[11]), msgStr + "\r\n");
  EXPECT_EQ(res.at(clients[12]), msgStr + "\r\n");
  // 10には送信されていないことを確認
  EXPECT_EQ(res.find(clients[10]), res.end());
}
