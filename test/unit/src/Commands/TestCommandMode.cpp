#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"

static void makeUserData(IRCServer &server, std::map<int, Client *> &clients) {
  clients[10] = new Client(10);
  clients[10]->setNickName("nick1");
  clients[10]->setUserName("user1");
  clients[10]->setIsRegistered(true);

  clients[11] = new Client(11);
  clients[11]->setNickName("nick2");
  clients[11]->setUserName("user2");
  clients[11]->setIsRegistered(true);

  clients[12] = new Client(12);
  clients[12]->setNickName("nick3");
  clients[12]->setUserName("user3");
  clients[12]->setIsRegistered(true);

  server.addClient(clients[10]);
  server.addClient(clients[11]);
  server.addClient(clients[12]);
}

static void makeChannelData(IRCServer &server,
                            std::map<int, Client *> &clients) {
  server.addChannel("#test", clients[10]);
  Channel *channel = server.getChannel("#test");

  channel->addChanop(clients[10]);
  clients[10]->addJoinedChannel(channel);
  channel->addMember(clients[11]);
  channel->addChanop(clients[11]);
  clients[11]->addJoinedChannel(channel);
  channel->addMember(clients[12]);
  channel->addChanop(clients[12]);
  clients[12]->addJoinedChannel(channel);
}

// 正常
TEST(CommandMode, nomal1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);
  makeChannelData(server, clients);
  RequestHandler requestHandler(&server);

  std::string msgStr = "MODE #test +i";
  IRCMessage msg(clients[10], msgStr);

  requestHandler.handleCommand(msg);

  std::string expected = ":nick1!~user1@localhost MODE #test +i";

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), expected + "\r\n");

  // チャンネルの状態を確認
  Channel *channel = server.getChannel("#test");
  EXPECT_TRUE(channel->getIsInviteOnly());
}
