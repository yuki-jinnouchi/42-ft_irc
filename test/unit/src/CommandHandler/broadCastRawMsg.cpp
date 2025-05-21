#include <gtest/gtest.h>

#include "CommandHandler.hpp"
#include "IRCServer.hpp"

TEST(CommandHandler, broadCastRawMsg) {
  IRCServer server("6677", "pass123");

  std::map<int, Client*> clients;
  clients[10] = new Client(10);
  clients[11] = new Client(11);
  clients[12] = new Client(12);
  server.addClient(clients[10]);
  server.addClient(clients[11]);
  server.addClient(clients[12]);

  std::string msgStr = "abcdefg";
  IRCMessage msg(clients[10], msgStr);

  CommandHandler commandHandler(&server);
  const std::map<Client*, std::string>& res =
      commandHandler.broadCastRawMsg(msg);

  EXPECT_EQ(res.size(), 2);

  // 11, 12にそのままのメッセージが送信されていることを確認
  EXPECT_EQ(res.at(clients[11]), msgStr + "\r\n");
  EXPECT_EQ(res.at(clients[12]), msgStr + "\r\n");
  // 10には送信されていないことを確認
  EXPECT_EQ(res.find(clients[10]), res.end());
}
