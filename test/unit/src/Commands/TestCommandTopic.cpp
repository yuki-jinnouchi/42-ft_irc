#include <gtest/gtest.h>

#include "IRCServer.hpp"
#include "RequestHandler.hpp"
#include "TestDataGenerator.hpp"

// 正常（設定＆取得）
TEST(CommandTopic, nomal1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // トピックの設定
  std::string msgStr1 = "TOPIC #ch3 :Awesome Channel Topic";
  IRCMessage msg1(clients[11], msgStr1);
  requestHandler.handleCommand(msg1);

  // トピックの取得
  std::string msgStr2 = "TOPIC #ch3";
  IRCMessage msg2(clients[11], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected1 =
      ":nick2!~user2@localhost TOPIC #ch3 :Awesome Channel Topic";
  std::string expected2 =
      ":irc.example.net 332 nick2 #ch3 :Awesome Channel Topic";

  // トピックが設定される
  EXPECT_EQ(server.getChannel("#ch3")->getTopic(), "Awesome Channel Topic");

  // 変更者（オペレーターではない）
  EXPECT_EQ(clients[11]->getSendingMsg(),
            expected1 + "\r\n" + expected2 + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[10]->getSendingMsg(), expected1 + "\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), expected1 + "\r\n");
}

// 正常（チャンネルがTモードかつ権限がない場合の取得）
TEST(CommandTopic, mode_t_not_op_get) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // Tモードに設定
  TestDataGenerator::callCommand(requestHandler, clients[10], "MODE #ch3 +t");
  // トピック設定
  TestDataGenerator::callCommand(requestHandler, clients[10],
                                 "TOPIC #ch3 :Awesome Channel Topic");
  // 送信メッセージクリア
  clients[10]->consumeSendingMsg(100000);
  clients[11]->consumeSendingMsg(100000);
  clients[12]->consumeSendingMsg(100000);

  // トピックの設定
  std::string msgStr2 = "TOPIC #ch3";
  IRCMessage msg2(clients[11], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected =
      ":irc.example.net 332 nick2 #ch3 :Awesome Channel Topic";

  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 正常（チャンネルがTモードかつ権限がある場合の設定）
TEST(CommandTopic, mode_t_have_op_set) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // Tモードに設定
  TestDataGenerator::callCommand(requestHandler, clients[10], "MODE #ch3 +t");
  // 送信メッセージクリア
  clients[10]->consumeSendingMsg(100000);
  clients[11]->consumeSendingMsg(100000);
  clients[12]->consumeSendingMsg(100000);

  // トピックの設定
  std::string msgStr2 = "TOPIC #ch3 :Awesome Channel Topic";
  IRCMessage msg2(clients[10], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected =
      ":nick1!~user1@localhost TOPIC #ch3 :Awesome Channel Topic";

  // トピックが設定される
  EXPECT_EQ(server.getChannel("#ch3")->getTopic(), "Awesome Channel Topic");

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), expected + "\r\n");
}

// 未ログイン
TEST(CommandTopic, not_registered) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // トピックの設定
  std::string msgStr1 = "TOPIC #ch3 :Awesome Channel Topic";
  IRCMessage msg1(clients[15], msgStr1);
  requestHandler.handleCommand(msg1);

  // トピックの取得
  std::string msgStr2 = "TOPIC #ch3";
  IRCMessage msg2(clients[15], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected = ":irc.example.net 451 * :You have not registered";

  // トピックが設定されない
  EXPECT_EQ(server.getChannel("#ch3")->getTopic(), "");
  // 変更者（オペレーターではない）
  EXPECT_EQ(clients[15]->getSendingMsg(),
            expected + "\r\n" + expected + "\r\n");
  // チャンネル参加者(通知されない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 引数不足
TEST(CommandTopic, arg0) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "topic";
  std::string expected =
      ":irc.example.net 461 nick2 TOPIC :Not enough parameters";

  IRCMessage msg(clients[11], msgStr);
  requestHandler.handleCommand(msg);

  // 変更者（オペレーターではない）
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者(通知されない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 引数過剰
TEST(CommandTopic, arg3) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "topic #ch3 awesome topick";
  std::string expected =
      ":irc.example.net 461 nick2 TOPIC :Not enough parameters";

  IRCMessage msg(clients[11], msgStr);
  requestHandler.handleCommand(msg);

  // トピックが設定されない
  EXPECT_EQ(server.getChannel("#ch3")->getTopic(), "");
  // 変更者（オペレーターではない）
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者(通知されない)
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// トピックがない場合の取得
TEST(CommandTopic, no_topic) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // トピックの取得
  std::string msgStr2 = "TOPIC #ch3";
  IRCMessage msg2(clients[11], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected = ":irc.example.net 331 nick2 #ch3 :No topic is set";

  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// チャンネルがない場合の取得
TEST(CommandTopic, no_channel) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // トピックの取得
  std::string msgStr2 = "TOPIC #chx";
  IRCMessage msg2(clients[11], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected = ":irc.example.net 403 nick2 #chx :No such channel";

  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// チャンネルに参加していない場合の取得
TEST(CommandTopic, not_member_get) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // トピックの取得
  std::string msgStr2 = "TOPIC #ch5";
  IRCMessage msg2(clients[11], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected =
      ":irc.example.net 442 nick2 #ch5 :You're not on that channel";

  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// チャンネルに参加していない場合の設定
TEST(CommandTopic, not_member_set) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // トピックの設定
  std::string msgStr2 = "TOPIC #ch5 :topic for ch4";
  IRCMessage msg2(clients[11], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected =
      ":irc.example.net 442 nick2 #ch5 :You're not on that channel";

  // トピックが設定されない
  EXPECT_EQ(server.getChannel("#ch5")->getTopic(), "");

  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// チャンネルがTモードかつ権限がない場合の設定
TEST(CommandTopic, mode_t_not_op_set) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  // Tモードに設定
  TestDataGenerator::callCommand(requestHandler, clients[10], "MODE #ch3 +t");
  // 送信メッセージクリア
  clients[10]->consumeSendingMsg(100000);
  clients[11]->consumeSendingMsg(100000);
  clients[12]->consumeSendingMsg(100000);

  // トピックの設定
  std::string msgStr2 = "TOPIC #ch3 :Awesome Channel Topic";
  IRCMessage msg2(clients[11], msgStr2);
  requestHandler.handleCommand(msg2);

  std::string expected =
      ":irc.example.net 482 nick2 #ch3 :You're not channel operator";

  // トピックが設定されない
  EXPECT_EQ(server.getChannel("#ch3")->getTopic(), "");

  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // チャンネル参加者
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}
