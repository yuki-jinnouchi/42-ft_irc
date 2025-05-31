#include "RequestHandler.hpp"

#include "ACommand.hpp"
#include "CommandNick.hpp"
#include "CommandPass.hpp"
#include "CommandPing.hpp"
#include "CommandUser.hpp"
#include "IRCLogger.hpp"
#include "IRCParser.hpp"
#include "IRCServer.hpp"

// Orthodox Cannonical Form
RequestHandler::RequestHandler(IRCServer* server) : server_(server) {
  registerCommands();
}

RequestHandler::~RequestHandler() {
  unregisterCommands();
}

RequestHandler::RequestHandler(const RequestHandler& other) {
  *this = other;
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other) {
  if (this == &other) {
    return *this;
  }
  server_ = other.server_;
  commands_ = other.commands_;
  return *this;
}

void RequestHandler::registerCommands() {
  // commands_["CAP"] = new CommandCap(server_, "CAP");
  commands_["PASS"] = new CommandPass(server_, "PASS");
  commands_["NICK"] = new CommandNick(server_, "NICK");
  commands_["USER"] = new CommandUser(server_, "USER");
  // commands_["JOIN"] = new CommandJoin(server_, "JOIN");
  // commands_["PART"] = new CommandPart(server_, "PART");
  // commands_["PRIVMSG"] = new CommandPrivmsg(server_, "PRIVMSG");
  // commands_["QUIT"] = new CommandQuit(server_, "QUIT");
  commands_["PING"] = new CommandPing(server_, "PING");
  // commands_["PONG"] = new CommandPong(server_, "PONG");
}

void RequestHandler::unregisterCommands() {
  for (std::map<std::string, ACommand*>::iterator it = commands_.begin();
       it != commands_.end(); ++it) {
    delete it->second;
  }
  commands_.clear();
}

// Member functions
void RequestHandler::handleCommand(IRCMessage& msg) {
  IRCParser::parseRaw(msg);
  DEBUG_MSG("[COMMAND] " << " fd: " << msg.getFrom()->getFd()
                         << ", msg: " << msg.getRaw() << std::endl
                         << "                 "
                         << " prefix: " << msg.getPrefix()
                         << ", command: " << msg.getCommand()
                         << ", param[0]: " << msg.getParam(0)
                         << ", param[1]: " << msg.getParam(1));
  execCommand(msg);
}

ACommand* RequestHandler::getCommand(const std::string& commandName) {
  std::map<std::string, ACommand*>::iterator it = commands_.find(commandName);
  if (it != commands_.end()) return it->second;
  return 0;
}

void RequestHandler::execCommand(IRCMessage& msg) {
  std::string commandName = msg.getCommand();
  ACommand* command = getCommand(commandName);
  if (command)
    command->execute(msg);
  // else
  // TODO: コマンドがない場合のレスポンス送信が必要
  // それ用のコマンド"NULL"みたいなの作る？
}

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
      it->second->pushSendingMsg(msg.getRaw() +"\r\n");
      server_->addSendQueue(it->second);
    }
  }
  return msg.getResponses();
}
