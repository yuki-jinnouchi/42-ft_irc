#include "IRCServer.hpp"

#include <gtest/gtest.h>

#include "TestDataGenerator.hpp"

TEST(IRCServerTest, valid_construnctor) {
  {
    const char* port = "6667";
    const char* password = "password";
    IRCServer server(port, password);
    EXPECT_EQ(server.getPort(), std::string(port));
    EXPECT_EQ(server.getPassword(), std::string(password));
  }
  {
    const char* port = "1";
    const char* password = "p";
    IRCServer server(port, password);
    EXPECT_EQ(server.getPort(), std::string(port));
    EXPECT_EQ(server.getPassword(), std::string(password));
  }
  {
    const char* port = "65535";
    const char* password =
        "1234567890123456789012345678901234567890123456789012345678901234567890"
        "123456789012345678901234567890";
    IRCServer server(port, password);
    EXPECT_EQ(server.getPort(), std::string(port));
    EXPECT_EQ(server.getPassword(), std::string(password));
  }
}
TEST(IRCServerTest, invalid_port) {
  {
    const char* port = "0";
    const char* password = "password";
    try {
      IRCServer server(port, password);
    } catch (const std::invalid_argument& e) {
      EXPECT_STREQ(e.what(), "invalid port number");
    }
  }
  {
    const char* port = "65536";  // 65535より大きい
    const char* password = "password";
    try {
      IRCServer server(port, password);
    } catch (const std::invalid_argument& e) {
      EXPECT_STREQ(e.what(), "invalid port number");
    }
  }
  {
    const char* port = "6667a";
    const char* password = "password";
    try {
      IRCServer server(port, password);
    } catch (const std::invalid_argument& e) {
      EXPECT_STREQ(e.what(), "invalid port number");
    }
  }
  {
    const char* port = "";
    const char* password = "password";
    try {
      IRCServer server(port, password);
    } catch (const std::invalid_argument& e) {
      EXPECT_STREQ(e.what(), "invalid port number");
    }
  }
}
TEST(IRCServerTest, invalid_password) {
  {
    const char* port = "6667";
    const char* password = "";
    try {
      IRCServer server(port, password);
    } catch (const std::invalid_argument& e) {
      EXPECT_STREQ(e.what(), "invalid password");
    }
  }
  {
    const char* port = "6667";
    const char* password =
        "1234567890123456789012345678901234567890123456789012345678901234567890"
        "1234567890123456789012345678901";
    try {
      IRCServer server(port, password);
    } catch (const std::invalid_argument& e) {
      EXPECT_STREQ(e.what(), "invalid password");
    }
  }
}

TEST(IRCServerTest, disconnect) {
  IRCServer server("6677", "pass123");
  std::map<int, Client*> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // クライアントを切断
  server.disconnectClient(clients[10]);
  server.disconnectClient(clients[11]);
  server.disconnectClient(clients[12]);

  // クライアントが削除されている
  EXPECT_EQ(server.getClients().find(10), server.getClients().end());
  EXPECT_EQ(server.getClients().find(11), server.getClients().end());
  EXPECT_EQ(server.getClients().find(12), server.getClients().end());

  // 誰もいなくなったチャンネルが削除されている
  EXPECT_TRUE(server.getChannel("#ch1") != NULL);
  EXPECT_TRUE(server.getChannel("#ch2") == NULL);
  EXPECT_TRUE(server.getChannel("#ch3") == NULL);
  EXPECT_TRUE(server.getChannel("#ch4") != NULL);
}
