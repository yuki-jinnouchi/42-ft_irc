#include "CommandHandler.hpp"
#include "IRCLogger.hpp"

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

void CommandHandler::broadCastRawMsg(const IRCMessage& msg) {
  for (std::map<int, ClientSession*>::iterator it =
           server_->getClients().begin();
       it != server_->getClients().end(); ++it) {
    if (msg.isFromMe(it->second)) {
      // 自分自身はスキップ
      continue;
    } else {
      // 受信したデータを他のクライアントにそのまま送信
      it->second->sendMessage(msg.getRaw());
    }
  }
}

void CommandHandler::handleCommand(const IRCMessage& msg) {
  DEBUG_MSG("CommandHandler::handleCommand from: "
            << msg.getFrom()->getFd() << std::endl
            << "----------------------" << std::endl
            << msg.getRaw() << "----------------------");

  // TODO コマンドを解析して処理を分岐
  // 受信したデータを他のクライアントにそのまま送信
  broadCastRawMsg(msg);
}
