#include "RequestHandler.hpp"

#include "IRCLogger.hpp"
#include "IRCParser.hpp"
#include "command/CommandPass.hpp"
#include "command/CommandPing.hpp"

// Orthodox Cannonical Form
RequestHandler::RequestHandler(IRCServer* server) : server_(server) {
  initializeCommands();
}

RequestHandler::~RequestHandler() {}

RequestHandler::RequestHandler(const RequestHandler& other) {
  *this = other;
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other) {
  if (this == &other) {
    return *this;
  }
  server_ = other.server_;
  return *this;
}

void RequestHandler::initializeCommands() {
  // commands_["CAP"] = new CommandCap(server_, "CAP");
  commands_["PASS"] = new CommandPass(server_);
  // commands_["NICK"] = new CommandNick(server_, "NICK");
  // commands_["USER"] = new CommandUser(server_, "USER");
  // commands_["JOIN"] = new CommandJoin(server_, "JOIN");
  // commands_["PART"] = new CommandPart(server_, "PART");
  // commands_["PRIVMSG"] = new CommandPrivmsg(server_, "PRIVMSG");
  // commands_["QUIT"] = new CommandQuit(server_, "QUIT");
  commands_["PING"] = new CommandPing(server_);
  // commands_["PONG"] = new CommandPong(server_, "PONG");
}

// Member functions
void RequestHandler::handleCommand(IRCMessage& msg) {
  IRCParser::parseRaw(msg);
  DEBUG_MSG("[COMMAND] " << "fd: " << msg.getFrom()->getFd()
                         << ", msg: " << msg.getRaw() << std::endl
                         << "                "
                         << " prefix: " << msg.getPrefix()
                         << ", command: " << msg.getCommand()
                         << ", param[0]: " << msg.getParam(0)
                         << ", param[1]: " << msg.getParam(1));

  if (commands_.find(msg.getCommand()) != commands_.end()) {
    // コマンドが登録されている場合は実行
    commands_[msg.getCommand()]->execute(msg);
  } else {
    // 登録されていないコマンドの場合
    DEBUG_MSG("Unknown command: " << msg.getCommand());
  }
  return;
}

/*
const std::map<Client*, std::string>& RequestHandler::broadCastRawMsg(
    IRCMessage& msg) {
  for (std::map<int, Client*>::const_iterator it =
           server_->getClients().begin();
       it != server_->getClients().end(); ++it) {
    if (msg.isFromMe(it->second)) {
      // 自分自身はスキップ
      continue;
    } else {
      // 受信したデータを他のクライアントにそのまま送信
      msg.addResponse(it->second, msg.getRaw());
    }
  }
  return msg.getResponses();
}
*/
