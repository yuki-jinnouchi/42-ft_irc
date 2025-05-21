#include "CommandHandler.hpp"

#include "IRCLogger.hpp"

// Orthodox Cannonical Form
CommandHandler::CommandHandler(IRCServer* server) : server_(server) {}

CommandHandler::~CommandHandler() {}

CommandHandler::CommandHandler(const CommandHandler& other) { *this = other; }

CommandHandler& CommandHandler::operator=(const CommandHandler& other) {
  if (this == &other) {
    return *this;
  }
  server_ = other.server_;
  return *this;
}

// Member functions
const std::map<ClientSession*, std::string>& CommandHandler::broadCastRawMsg(IRCMessage& msg) {
  for (std::map<int, ClientSession*>::const_iterator it =
           server_->getClients().begin();
       it != server_->getClients().end(); ++it) {
    if (msg.isFromMe(it->second)) {
      // 自分自身はスキップ
      continue;
    } else {
      // 受信したデータを他のクライアントにそのまま送信
      msg.addResponse(it->second, msg.getRaw() + "\r\n");
    }
  }
  return msg.getResponses();
}

const std::map<ClientSession*, std::string>& CommandHandler::handleNick(IRCMessage& msg) {
  // TODO NICKコマンドの処理をちゃんと書く
  msg.getFrom()->setNickName("nick1");
  return msg.getResponses();
}

const std::map<ClientSession*, std::string>& CommandHandler::handlePing(IRCMessage& msg) {
  // TODO PINGコマンドの処理をちゃんと書く
  msg.addResponse(msg.getFrom(), "PONG\r\n");
  return msg.getResponses();
}

const std::map<ClientSession*, std::string>& CommandHandler::handleCommand(
    IRCMessage& msg) {
  IRCParser::parseRaw(msg);
  DEBUG_MSG("CommandHandler::handleCommand from: "
            << msg.getFrom()->getFd() << std::endl
            << "----------------------" << std::endl
            << msg.getRaw() << std::endl
            << "prefix: " << msg.getPrefix() << std::endl
            << "command: " << msg.getCommand() << std::endl
            << "param[0]: " << msg.getParam(0) << std::endl
            << "param[1]: " << msg.getParam(1) << std::endl
            << "----------------------");

  // TODO コマンドを解析して処理を分岐
  if (msg.getRaw().compare(0, 4, "NICK") == 0) {
    return handleNick(msg);
  } else if (msg.getRaw().compare(0, 4, "PING") == 0) {
    return handlePing(msg);
  }
  // 受信したデータを他のクライアントにそのまま送信
  return broadCastRawMsg(msg);
}
