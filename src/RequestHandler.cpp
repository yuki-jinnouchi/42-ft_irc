#include "RequestHandler.hpp"

#include "ACommand.hpp"
#include "CommandBroadCast.hpp"
#include "CommandNick.hpp"
#include "CommandPass.hpp"
#include "CommandPing.hpp"
#include "CommandPrivMsg.hpp"
#include "CommandUser.hpp"
#include "CommandJoin.hpp"
#include "IRCLogger.hpp"
#include "IRCParser.hpp"
#include "IRCServer.hpp"

// Orthodox Cannonical Form
RequestHandler::RequestHandler(IRCServer* server) : server_(server) {
  // commands_["CAP"] = new CommandCap(server_, "CAP");
  commands_["PASS"] = new CommandPass(server_);
  commands_["NICK"] = new CommandNick(server_);
  commands_["USER"] = new CommandUser(server_);
  commands_["JOIN"] = new CommandJoin(server_);
  // commands_["PART"] = new CommandPart(server_, "PART");
  commands_["PRIVMSG"] = new CommandPrivMsg(server_);
  // commands_["QUIT"] = new CommandQuit(server_, "QUIT");
  commands_["PING"] = new CommandPing(server_);
  // commands_["PONG"] = new CommandPong(server_, "PONG");
  commands_["BROADCAST"] = new CommandBroadCast(server_);
}

RequestHandler::~RequestHandler() {
  for (std::map<std::string, ACommand*>::iterator it = commands_.begin();
       it != commands_.end(); ++it) {
    delete it->second;
  }
  commands_.clear();
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
  return NULL;
}

void RequestHandler::execCommand(IRCMessage& msg) {
  std::string commandName = msg.getCommand();
  ACommand* command = getCommand(commandName);
  if (command) {
    command->execute(msg);
  } else {
    commands_["BROADCAST"]->execute(msg);
    // TODO ngircdのコマンドが見つからない場合のエラーメッセージ
    // hoge
    // 　:irc.example.net 421 nick1 hoge :Unknown command
  }
}
