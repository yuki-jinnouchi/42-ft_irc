#include "CommandHandler.hpp"

// Orthodox Cannonical Form
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

// Member functions
const std::map<Client*, std::string>& CommandHandler::handleCommand(
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

  // コマンドごとに処理を分岐
  // TODO: switch文など、もっと簡単に書けそう
  std::string command = msg.getCommand();
  DEBUG_MSG("Command: " << command);
  if (command == "NICK")
    Nick(msg);
  else if (command == "USER")
    User(msg);
  // else if (command == "JOIN")
  //   Join(msg);
  // else if (command == "PRIVMSG")
  //   Privmsg(msg);
  // else if (command == "PART")
  //   Part(msg);
  // else if (command == "QUIT")
  //   Quit(msg);
  else if (command == "PING")
    Ping(msg);

  // TODO: add commands
  /*
    KICK, INVITE, TOPIC, MODE
  */

  // 受信したデータを他のクライアントにそのまま送信
  else
    broadCastRawMsg(msg);
  DEBUG_MSG(command);
  return msg.getResponses();
}

void CommandHandler::Pass(const IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (from->getPassword() == "") msg.getFrom()->setPassword(msg.getParam(0));
}

void CommandHandler::Nick(const IRCMessage& msg) {
  // TODO: Valid nick (dup)
  Client* from = msg.getFrom();
  from->setNickName(msg.getParam(0));
}

void CommandHandler::User(const IRCMessage& msg) {
  Client* from = msg.getFrom();
  // <hostname> <servername>　を無視
  // TODO: Valid入れる？
  from->setNickName(msg.getParam(0));  //
  from->setRealName(msg.getParam(4));
}

void CommandHandler::Join(const IRCMessage& msg) {
  Client* client = msg.getFrom();
  std::string channelName = msg.getParam(0);
  if (server_->ifChannleExists(channelName) == false)
    server_->addChannel(channelName, client);
  else
    server_->getChannel(channelName)->addMember(client);
}

// void CommandHandler::Privmsg(const IRCMessage& msg) {
//   // TODO: 追って追記
// }

// void CommandHandler::Part(const IRCMessage& msg) {
//   // TODO: 追って追記
// }

// void CommandHandler::Quit(const IRCMessage& msg) {
//   // TODO: 追って追記
// }

void CommandHandler::Ping(IRCMessage& msg) {
  msg.addResponse(msg.getFrom(), "PONG" + msg.getParam(0) + "\r\n");
}

const std::map<Client*, std::string>& CommandHandler::broadCastRawMsg(
    IRCMessage& msg) {
  for (std::map<int, Client*>::const_iterator it =
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
