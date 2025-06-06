
#include "TestDataGenerator.hpp"

void TestDataGenerator::callCommand(RequestHandler &rh, Client *client,
                                    const std::string &command) {
  IRCMessage msg(client, command);
  rh.handleCommand(msg);
}

void TestDataGenerator::makeUserData(IRCServer &server,
                                     std::map<int, Client *> &clients,
                                     RequestHandler &rh) {
  clients[10] = new Client(10);
  clients[11] = new Client(11);
  clients[12] = new Client(12);
  clients[13] = new Client(13);
  clients[14] = new Client(14);
  clients[15] = new Client(15);
  server.addClient(clients[10]);
  server.addClient(clients[11]);
  server.addClient(clients[12]);
  server.addClient(clients[13]);
  server.addClient(clients[14]);
  server.addClient(clients[15]);

  callCommand(rh, clients[10], "USER user1 0 * :Real Name 1");
  callCommand(rh, clients[10], "PASS pass123");
  callCommand(rh, clients[10], "NICK nick1");
  callCommand(rh, clients[10], "JOIN #ch1");
  callCommand(rh, clients[10], "JOIN #ch2");
  callCommand(rh, clients[10], "JOIN #ch3");

  callCommand(rh, clients[11], "USER user2 0 * :Real Name 2");
  callCommand(rh, clients[11], "PASS pass123");
  callCommand(rh, clients[11], "NICK nick2");
  callCommand(rh, clients[11], "JOIN #ch2");
  callCommand(rh, clients[11], "JOIN #ch3");
  callCommand(rh, clients[11], "JOIN #ch4");

  callCommand(rh, clients[12], "USER user3 0 * :Real Name 3");
  callCommand(rh, clients[12], "PASS pass123");
  callCommand(rh, clients[12], "NICK nick3");
  callCommand(rh, clients[12], "JOIN #ch3");
  callCommand(rh, clients[12], "JOIN #ch4");
  callCommand(rh, clients[12], "JOIN #ch5");

  callCommand(rh, clients[13], "USER user4 0 * :Real Name 4");
  callCommand(rh, clients[13], "PASS pass123");
  callCommand(rh, clients[13], "NICK nick4");
  callCommand(rh, clients[13], "JOIN #ch4");
  callCommand(rh, clients[13], "JOIN #ch5");
  callCommand(rh, clients[13], "JOIN #ch1");

  // 未ログイン（ニックネームあり）
  callCommand(rh, clients[14], "USER user5 0 * :Real Name 5");
  // callCommand(rh, clients[14], "PASS pass123");
  callCommand(rh, clients[14], "NICK nick5");

  // 未ログイン（ニックネームなし）
  callCommand(rh, clients[15], "USER user6 0 * :Real Name 6");
  callCommand(rh, clients[15], "PASS pass123");
  // callCommand(rh, clients[15], "NICK nick6");

  clients[10]->consumeSendingMsg(100000);  // 送信メッセージをクリア
  clients[11]->consumeSendingMsg(100000);  // 送信メッセージをクリア
  clients[12]->consumeSendingMsg(100000);  // 送信メッセージをクリア
  clients[13]->consumeSendingMsg(100000);  // 送信メッセージをクリア
  clients[14]->consumeSendingMsg(100000);  // 送信メッセージをクリア
  clients[15]->consumeSendingMsg(100000);  // 送信メッセージをクリア

  clients[10]->popReceivingMsg();  // 受信メッセージをクリア
  clients[11]->popReceivingMsg();  // 受信メッセージをクリア
  clients[12]->popReceivingMsg();  // 受信メッセージをクリア
  clients[13]->popReceivingMsg();  // 受信メッセージをクリア
  clients[14]->popReceivingMsg();  // 受信メッセージをクリア
  clients[15]->popReceivingMsg();  // 受信メッセージをクリア
}
