#include <gtest/gtest.h>

#include "TestDataGenerator.hpp"

// 通常（コメントあり）
TEST(CommandKick, nomal1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3 nick2 :bye bye";
  std::string expected = ":nick1!~user1@localhost KICK #ch3 nick2 :bye bye";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // メンバーが見つからない
  std::set<Client *> member = server.getChannel("#ch3")->getMember();
  EXPECT_EQ(member.find(clients[11]), member.end());

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // チャンネルの他のユーザーへの通知をテストする
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), expected + "\r\n");

  EXPECT_EQ(clients[13]->getSendingMsg(), "");
}

// 通常(コメントなし)
TEST(CommandKick, nomal2) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3 nick2";
  std::string expected = ":nick1!~user1@localhost KICK #ch3 nick2 :nick1";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // メンバーが見つからない
  std::set<Client *> member = server.getChannel("#ch3")->getMember();
  EXPECT_EQ(member.find(clients[11]), member.end());

  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // チャンネルの他のユーザーへの通知をテストする
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  EXPECT_EQ(clients[12]->getSendingMsg(), expected + "\r\n");

  EXPECT_EQ(clients[13]->getSendingMsg(), "");
}

// ログインしていない
TEST(CommandKick, not_registered) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3 nick2";
  std::string expected = ":irc.example.net 451 * :You have not registered";

  IRCMessage msg(clients[15], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[15]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 引数エラー //////////////////////////////////////////////////////////

// 引数なし
TEST(CommandKick, no_arg) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK";
  std::string expected =
      ":irc.example.net 461 nick1 KICK :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 引数不足
TEST(CommandKick, arg1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3";
  std::string expected =
      ":irc.example.net 461 nick1 KICK :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 引数過剰
TEST(CommandKick, arg4) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3 nick2 bye bye";
  std::string expected =
      ":irc.example.net 461 nick1 KICK :Not enough parameters";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// チャンネルエラー //////////////////////////////////////////////////////////

// チャンネルが存在しない
TEST(CommandKick, not_exit_ch) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #chx nick2 :bye bye";
  std::string expected = ":irc.example.net 403 nick1 #chx :No such channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 実行者がチャンネルに所属していない
TEST(CommandKick, I_am_not_member) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch4 nick2 :bye bye";
  std::string expected =
      ":irc.example.net 442 nick1 #ch4 :You're not on that channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 　権限がない（チャンネルのオペレーターではない）
TEST(CommandKick, no_operator) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3 nick2 :bye bye";
  std::string expected =
      ":irc.example.net 482 nick2 #ch3 :You're not channel operator";

  IRCMessage msg(clients[11], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[11]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[10]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// クライアントエラー //////////////////////////////////////////////////////////

// ニックネームが存在しない
TEST(CommandKick, not_exit_nick) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3 xxx :bye bye";
  std::string expected = ":irc.example.net 401 nick1 xxx :No such nick/channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 対象者がチャンネルに参加していない
TEST(CommandKick, nick_is_not_member) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr = "KICK #ch3 nick4 :bye bye";
  std::string expected =
      ":irc.example.net 441 nick1 nick4 #ch3 :They aren't on that channel";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // エラーメッセージ
  EXPECT_EQ(clients[10]->getSendingMsg(), expected + "\r\n");
  // 何も送信されない
  EXPECT_EQ(clients[11]->getSendingMsg(), "");
  EXPECT_EQ(clients[12]->getSendingMsg(), "");
}

// 複数 //////////////////////////////////////////////////////////

// 複数チャンネル、複数ニックネーム
TEST(CommandKick, nomal_multip1) {
  IRCServer server("6677", "pass123");
  std::map<int, Client *> clients;
  RequestHandler requestHandler(&server);
  TestDataGenerator::makeUserData(server, clients, requestHandler);

  std::string msgStr =
      "KICK #ch2,#ch3,#ch4,#chx nick2,xxx,nick3,nick4 :bye bye";

  IRCMessage msg(clients[10], msgStr);
  requestHandler.handleCommand(msg);

  // メンバーが見つからない
  std::set<Client *> member = server.getChannel("#ch3")->getMember();
  EXPECT_EQ(member.find(clients[11]), member.end());

  EXPECT_EQ(
      clients[10]->getSendingMsg(),
      ":nick1!~user1@localhost KICK #ch2 nick2 :bye bye\r\n"
      ":irc.example.net 401 nick1 xxx :No such nick/channel\r\n"
      ":irc.example.net 441 nick1 nick3 #ch2 :They aren't on that channel\r\n"
      ":irc.example.net 441 nick1 nick4 #ch2 :They aren't on that channel\r\n"
      ":nick1!~user1@localhost KICK #ch3 nick2 :bye bye\r\n"
      ":irc.example.net 401 nick1 xxx :No such nick/channel\r\n"
      ":nick1!~user1@localhost KICK #ch3 nick3 :bye bye\r\n"
      ":irc.example.net 441 nick1 nick4 #ch3 :They aren't on that channel\r\n"
      ":irc.example.net 442 nick1 #ch4 :You're not on that channel\r\n"
      ":irc.example.net 403 nick1 #chx :No such channel\r\n");
  // チャンネルの他のユーザーへの通知をテストする
  EXPECT_EQ(clients[11]->getSendingMsg(),
            ":nick1!~user1@localhost KICK #ch2 nick2 :bye bye\r\n"
            ":nick1!~user1@localhost KICK #ch3 nick2 :bye bye\r\n");

  EXPECT_EQ(clients[12]->getSendingMsg(),
            ":nick1!~user1@localhost KICK #ch3 nick2 :bye bye\r\n"
            ":nick1!~user1@localhost KICK #ch3 nick3 :bye bye\r\n");
  EXPECT_EQ(clients[13]->getSendingMsg(), "");
  EXPECT_EQ(clients[14]->getSendingMsg(), "");
}
