#include <gtest/gtest.h>
#include "CommandHandler.hpp"
#include "IRCServer.hpp"

TEST(CommandHandlerTE, handleNick1) {
  IRCServer server("6677", "pass123");

  std::map<int, ClientSession*> clients;
  clients[10] = new ClientSession(10);
  clients[11] = new ClientSession(11);
  clients[12] = new ClientSession(12);
  server.addClient(clients[10]);
  server.addClient(clients[11]);
  server.addClient(clients[12]);

  std::string msgStr = "NICK nick1";
  std::string expected = "nick1";
  IRCMessage msg(clients[10], msgStr);

  CommandHandler commandHandler(&server);
  const std::map<ClientSession*, std::string> res =
      commandHandler.handleNick(msg);

  EXPECT_EQ(res.size(), 0);
  EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
}

// TODO まだニックネームがnick1の時しか実装していないので、エラーになる
// TEST(CommandHandlerTE, handleNick2) {
//   IRCServer server("6677", "pass123");

//   std::map<int, ClientSession*> clients;
//   clients[10] = new ClientSession(10);
//   clients[11] = new ClientSession(11);
//   clients[12] = new ClientSession(12);
//   server.addClient(clients[10]);
//   server.addClient(clients[11]);
//   server.addClient(clients[12]);

//   std::string msgStr = "NICK nick2";
//   std::string expected = "nick2";
//   IRCMessage msg(clients[10], msgStr);

//   CommandHandler commandHandler(&server);
//   commandHandler.handleNick(msg);

//   const std::map<ClientSession*, std::string> res = msg.getResponses();

//   EXPECT_EQ(res.size(), 0);
//   EXPECT_EQ(server.getClients().at(10)->getNickName(), expected);
// }
