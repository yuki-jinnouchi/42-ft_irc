#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"
#include "TestDataGenerator.hpp"

// 大文字小文字混合のコマンドでログインテスト
TEST(RequestHandler, login) {
  IRCServer server("6677", "pass123");
  RequestHandler rh(&server);

  std::map<int, Client*> clients;
  clients[10] = new Client(10);
  server.addClient(clients[10]);

  TestDataGenerator::callCommand(rh, clients[10], "aBcDeF");
  TestDataGenerator::callCommand(rh, clients[10],
                                 "user user1 0 * :Real Name 1");
  TestDataGenerator::callCommand(rh, clients[10], "PaSs pass123");
  TestDataGenerator::callCommand(rh, clients[10], "nIck nick1");
  TestDataGenerator::callCommand(rh, clients[10], "joiN #ch1");
  TestDataGenerator::callCommand(rh, clients[10], "JOIN #ch2");
  TestDataGenerator::callCommand(rh, clients[10], "Abc #ch2");
  TestDataGenerator::callCommand(rh, clients[10], "123 #ch2");

  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 001 nick1 nick1 :Welcome to the Internet Relay "
            "Network nick1!\r\n"
            ":nick1!~user1@localhost JOIN #ch1\r\n"
            ":irc.example.net 332 nick1 #ch1 :\r\n"
            ":irc.example.net 353 nick1 #ch1 :nick1\r\n"
            ":irc.example.net 366 nick1 #ch1 :End of /NAMES list\r\n"
            ":nick1!~user1@localhost JOIN #ch2\r\n"
            ":irc.example.net 332 nick1 #ch2 :\r\n"
            ":irc.example.net 353 nick1 #ch2 :nick1\r\n"
            ":irc.example.net 366 nick1 #ch2 :End of /NAMES list\r\n"
            ":irc.example.net 421 nick1 Abc :Unknown command\r\n"
            ":irc.example.net 421 nick1 123 :Unknown command\r\n");
}
