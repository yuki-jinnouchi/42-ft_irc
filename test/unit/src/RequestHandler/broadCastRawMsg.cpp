#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"

TEST(RequestHandler, broadCastRawMsg) {
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

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 11, 12にそのままのメッセージが送信されていることを確認
  EXPECT_EQ(clients[11]->getSendingMsg(), msgStr + "\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), msgStr + "\r\n");
  // 10には送信されていないことを確認
  EXPECT_TRUE(clients[10]->getSendingMsg().empty());
}
