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
  // TODO 他のユーザーをチャンネルに参加させる
}

// 通常
TEST(CommandPrivMsg, nomal1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG nick2 hello";
  std::string expected = ":nick1!~user1@localhost PRIVMSG nick2 :hello";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 通常（:だけの場合）
TEST(CommandPrivMsg, nomal2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG nick2 :";
  std::string expected = ":nick1!~user1@localhost PRIVMSG nick2 :";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 通常（通常（日本語、絵文字、スペース）
TEST(CommandPrivMsg, nomal3) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG nick2 :寿司🍣 酒池肉林🍺🍖 Hello World!";
  std::string expected =
      ":nick1!~user1@localhost PRIVMSG nick2 :寿司🍣 酒池肉林🍺🍖 Hello World!";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 通常（長いメッセージ（後ろカットされる）
TEST(CommandPrivMsg, nomal4) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

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

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(何も受け取らない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 通常(自分自身に送信)
TEST(CommandPrivMsg, nomal5) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG nick1 hello";
  std::string expected = ":nick1!~user1@localhost PRIVMSG nick1 :hello";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 未ログイン（ニックネームなし）
TEST(CommandPrivMsg, not_registered1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  clients[10]->setIsRegistered(false);  // 未ログイン状態にする
  clients[10]->setNickName("");         // ニックネームを空にする

  std::string msgStr = "PRIVMSG nick2 hello";
  std::string expected = ":irc.example.net 451 * :You have not registered";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 未ログイン（ニックネームあり）
TEST(CommandPrivMsg, not_registered2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  clients[10]->setIsRegistered(false);  // 未ログイン状態にする

  std::string msgStr = "PRIVMSG nick2 hello";
  std::string expected = ":irc.example.net 451 nick1 :You have not registered";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 引数不足
TEST(CommandPrivMsg, no_args) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG";
  std::string expected =
      ":irc.example.net 411 nick1 :No recipient given (PRIVMSG)";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 引数不足
TEST(CommandPrivMsg, one_args) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG nick2";
  std::string expected = ":irc.example.net 412 nick1 :No text to send";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 引数過剰
TEST(CommandPrivMsg, over_args) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG nick2 a b";
  std::string expected =
      ":irc.example.net 461 nick1 PRIVMSG :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  // TODO チャンネルの他のユーザーへの通知をテストする
}

// 存在しないニックネーム
TEST(CommandPrivMsg, not_exist) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  makeUserData(server, clients);

  std::string msgStr = "PRIVMSG xxx hello";
  std::string expected = ":irc.example.net 401 nick1 xxx :No such nick/channel";

  IRCMessage msg(clients[10], msgStr);

  RequestHandler requestHandler(&server);
  requestHandler.handleCommand(msg);

  // 送信者(エラーメッセージ)
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 受信者(何も受け取らない)
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  // TODO チャンネルの他のユーザーへの通知をテストする
}
