#include "CommandHandler.hpp"

CommandHandler::CommandHandler(IRCServer* server) : server_(server) {}
CommandHandler::~CommandHandler() {}
CommandHandler::CommandHandler(const CommandHandler& other) {
  *this = other;
}

CommandHandler& CommandHandler::operator=(const CommandHandler& other) {
  if (this == &other) {
    return *this;
  }
  server_ = other.server_;
  return *this;
}

void CommandHandler::handleCommand(const IRCMessage& msg) {
  std::map<int, ClientSession*> clients = server_->getClients();

  // TODO コマンドを解析して処理を分岐
  // 受信したデータを他のクライアントにそのまま送信
  for (std::map<int, ClientSession*>::iterator it = clients.begin();
       it != clients.end(); ++it) {
    if (msg.isFromMe(it->second)) {
      // 自分自身はスキップ
      continue;
    } else {
      // 受信したデータを他のクライアントにそのまま送信
      it->second->sendMessage(msg.getRaw());
    }
  }
}
