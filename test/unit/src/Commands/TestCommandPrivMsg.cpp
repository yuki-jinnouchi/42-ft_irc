#include <gtest/gtest.h>

#include "TestDataGenerator.hpp"

// 通常
TEST(CommandPrivMsg, nomal1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick2 hello";
  std::string expected = ":nick1!~user1@localhost PRIVMSG nick2 :hello";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
}

// 通常（:だけの場合）
TEST(CommandPrivMsg, nomal2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick2 :";
  std::string expected = ":nick1!~user1@localhost PRIVMSG nick2 :";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
}

// 通常（通常（日本語、絵文字、スペース）
TEST(CommandPrivMsg, nomal3) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick2 :寿司🍣 酒池肉林🍺🍖 Hello World!";
  std::string expected =
      ":nick1!~user1@localhost PRIVMSG nick2 :寿司🍣 酒池肉林🍺🍖 Hello World!";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
}

// 通常（長いメッセージ（後ろカットされる）
TEST(CommandPrivMsg, nomal4) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr =
      "PRIVMSG nick2 "
      "123456789012345678901234567890123456789012345678901234567890123456789012"
      "345678901234567890123456789012345678901234567890123456789012345678901234"
      "567890123456789012345678901234567890123456789012345678901234567890123456"
      "789012345678901234567890123456789012345678901234567890123456789012345678"
      "901234567890123456789012345678901234567890123456789012345678901234567890"
      "123456789012345678901234567890123456789012345678901234567890123456789012"
      "34567890123456789012345678901234567890123456789012345678901234";
  std::string expected =
      ":nick1!~user1@localhost PRIVMSG nick2 "
      ":12345678901234567890123456789012345678901234567890123456789012345678901"
      "234567890123456789012345678901234567890123456789012345678901234567890123"
      "456789012345678901234567890123456789012345678901234567890123456789012345"
      "678901234567890123456789012345678901234567890123456789012345678901234567"
      "890123456789012345678901234567890123456789012345678901234567890123456789"
      "012345678901234567890123456789012345678901234567890123456789012345678901"
      "23456789012345678901234567890123456[CUT]";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
}

// 通常(自分自身に送信)
TEST(CommandPrivMsg, nomal5) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick1 hello";
  std::string expected = ":nick1!~user1@localhost PRIVMSG nick1 :hello";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

// 未ログイン（ニックネームなし）
TEST(CommandPrivMsg, not_registered1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  clients[10]->setIsRegistered(false);  // 未ログイン状態にする
  clients[10]->setNickName("");         // ニックネームを空にする

  std::string msgStr = "PRIVMSG nick2 hello";
  std::string expected = ":irc.example.net 451 * :You have not registered";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

// 未ログイン（ニックネームあり）
TEST(CommandPrivMsg, not_registered2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  clients[10]->setIsRegistered(false);  // 未ログイン状態にする

  std::string msgStr = "PRIVMSG nick2 hello";
  std::string expected = ":irc.example.net 451 nick1 :You have not registered";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

// 引数不足
TEST(CommandPrivMsg, no_args) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG";
  std::string expected =
      ":irc.example.net 411 nick1 :No recipient given (PRIVMSG)";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

// 引数不足
TEST(CommandPrivMsg, one_args) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick2";
  std::string expected = ":irc.example.net 412 nick1 :No text to send";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

// 引数過剰
TEST(CommandPrivMsg, over_args) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick2 a b";
  std::string expected =
      ":irc.example.net 461 nick1 PRIVMSG :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

// 存在しないニックネーム
TEST(CommandPrivMsg, not_exist) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG xxx hello";
  std::string expected = ":irc.example.net 401 nick1 xxx :No such nick/channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

// 送信元が未ログイン
TEST(CommandPrivMsg, not_registered_sender) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick1 hello";
  std::string expected = ":irc.example.net 451 nick5 :You have not registered";
  // TODO

  IRCMessage msg(clients[14], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[14]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
}

// 受信者先が未ログイン
TEST(CommandPrivMsg, not_registered_reciver) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick5 hello";
  std::string expected =
      ":irc.example.net 401 nick1 nick5 :No such nick/channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
}

//=============================================================

// 通常（複数ニックネーム、複数チャンネル）
TEST(CommandPrivMsg, nomal_multi1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick2,nick3,#ch1,#ch2,#ch5 hello";
  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(),
            ":nick1!~user1@localhost PRIVMSG nick2 :hello\r\n"
            ":nick1!~user1@localhost PRIVMSG #ch2 :hello\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(),
            ":nick1!~user1@localhost PRIVMSG nick3 :hello\r\n"
            ":nick1!~user1@localhost PRIVMSG #ch5 :hello\r\n");
  EXPECT_EQ(clients[13]->getSendingMsg(),
            ":nick1!~user1@localhost PRIVMSG #ch1 :hello\r\n"
            ":nick1!~user1@localhost PRIVMSG #ch5 :hello\r\n");
  EXPECT_EQ(clients[14]->getSendingMsg(), "");
  EXPECT_EQ(clients[15]->getSendingMsg(), "");
}

// エラーあり（複数ニックネーム、複数チャンネル）
TEST(CommandPrivMsg, error_multi1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "PRIVMSG nick2,xxx,#ch1,#chx,nick5,nick6 hello";
  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // 送信者
  EXPECT_EQ(clients[10]->getSendingMsg(),
            ":irc.example.net 401 nick1 xxx :No such nick/channel\r\n"
            ":irc.example.net 401 nick1 #chx :No such nick/channel\r\n"
            ":irc.example.net 401 nick1 nick5 :No such nick/channel\r\n"
            ":irc.example.net 401 nick1 nick6 :No such nick/channel\r\n");

  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(),
            ":nick1!~user1@localhost PRIVMSG nick2 :hello\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
  EXPECT_EQ(clients[13]->getSendingMsg(),
            ":nick1!~user1@localhost PRIVMSG #ch1 :hello\r\n");
  EXPECT_EQ(clients[14]->getSendingMsg(), "");
  EXPECT_EQ(clients[15]->getSendingMsg(), "");
}
