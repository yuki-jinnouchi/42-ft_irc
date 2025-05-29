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
  DEBUG_MSG("[COMMAND] " << " fd: " << msg.getFrom()->getFd()
                         << ", msg: " << msg.getRaw() << std::endl
                         << "                 "
                         << " prefix: " << msg.getPrefix()
                         << ", command: " << msg.getCommand()
                         << ", param[0]: " << msg.getParam(0)
                         << ", param[1]: " << msg.getParam(1));

  // コマンドごとに処理を分岐
  // TODO: switch文など、もっと簡単に書けそう
  std::string command = msg.getCommand();
  // DEBUG_MSG("[EXECUTE]: " << command);
  // if (command == "CAP")
  //   cap(msg);
  if (command == "PASS")
    pass(msg);
  else if (command == "NICK")
    nick(msg);
  else if (command == "USER")
    user(msg);
  // else if (command == "JOIN")
  //   Join(msg);
  // else if (command == "PRIVMSG")
  //   Privmsg(msg);
  // else if (command == "PART")
  //   Part(msg);
  // else if (command == "QUIT")
  //   Quit(msg);
  else if (command == "PING")
    ping(msg);

  // TODO: add commands
  /*
    KICK, INVITE, TOPIC, MODE
  */

  // 受信したデータを他のクライアントにそのまま送信
  else
    broadCastRawMsg(msg);
  return msg.getResponses();
}

// void CommandHandler::cap(IRCMessage& msg) {
//   Client* from = msg.getFrom();
//   if (msg.getParam(0).empty()) {
//     msg.addResponse(msg.getFrom(), "461 CAP :Not enough parameters");
//   } else if (msg.getParam(0) == "LS") {  // CAP LS
//     from->setIsCapabilityNegotiating(true);
//     msg.addResponse(msg.getFrom(), "CAP * LS :multi-prefix");
//   } else if (msg.getParam(0) == "REQ") {  // CAP REQ
//     msg.addResponse(msg.getFrom(), "CAP * ACK :multi-prefix");
//   } else if (msg.getParam(0) == "END") {  // CAP END
//     msg.addResponse(msg.getFrom(), "CAP END");
//     from->setIsCapabilityNegotiating(false);
//   } else {
//     msg.addResponse(msg.getFrom(), "421 :Invalid CAP command");
//   }
// }

void CommandHandler::pass(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::string password = msg.getParam(0);
  // NOTE: ignore hop count
  if (password.empty()) {
    std::string errorMsg = "461 PASS :Not enough parameters";
    msg.addResponse(from, errorMsg);
    return;
  }
  if (from->getIsPasswordPassed()) {
    std::string errorMsg = "462 :You may not reregister";
    msg.addResponse(from, errorMsg);
    return;
  }
  if (password != server_->getPassword()) {
    std::string errorMsg = "464 :Password incorrect";
    msg.addResponse(from, errorMsg);
    // TODO:パスワードが間違っている場合には、クライアントを切断したい
    return;
  }
  from->setIsPasswordPassed(true);
}

void CommandHandler::nick(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (msg.getParam(0).empty()) {
    std::string errorMsg = "431 :No nickname given";
    msg.addResponse(from, errorMsg);
    return;
  }
  if (msg.getParam(0).size() > Client::kMaxNickNameSize) {
    std::string errorMsg = "432 :Nickname too long";
    msg.addResponse(from, errorMsg);
    return;
  }
  if (server_->isNickTaken(msg.getParam(0))) {
    std::string errorMsg =
        "433 " + msg.getParam(0) + " :Nickname is already in use";
    msg.addResponse(from, errorMsg);
    return;
  }
  // TODO: valid Nick characters (432 ERR_ERRONEUSNICKNAME)
  from->setNickName(msg.getParam(0));
  sendWelcome(msg);
}

void CommandHandler::user(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (msg.getParam(0).empty() || msg.getParam(3).empty()) {
    std::string errorMsg = "461 USER :Not enough parameters";
    return;
  }
  if (from->getIsRegistered()) {
    std::string errorMsg = "462 :You may not reregister";
    msg.addResponse(from, errorMsg);
    return;
  }
  // TODO: Valid入れる？
  from->setUserName(msg.getParam(0));
  from->setRealName(msg.getParam(3));
  sendWelcome(msg);
}

void CommandHandler::join(IRCMessage& msg) {
  Client* client = msg.getFrom();
  if (msg.getParam(0).empty() || msg.getParam(3).empty()) {
    std::string errorMsg = "461 JOIN :Not enough parameters";
    return;
  }
  // NOTE: now only with one public channel
  char firstChar = msg.getParam(0)[0];
  std::string channelName = msg.getParam(0).substr(1);
  if (firstChar != '#' && firstChar != '&') {
    std::string errorMsg = "403 " + channelName + " :No such channel";
    msg.addResponse(client, errorMsg);
    return;
  }
  // TODO: now ignore invitation and password
  if (!server_->ifChannleExists(channelName))
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

void CommandHandler::ping(IRCMessage& msg) {
  msg.addResponse(msg.getFrom(), "PONG " + msg.getParam(0));
}

void CommandHandler::sendWelcome(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (from->getIsRegistered() || !from->getIsPasswordPassed() ||
      from->getNickName().empty() || from->getUserName().empty()) {
    return;
  }
  from->setIsRegistered(true);
  std::string welcomeMsg = "001 " + from->getNickName() +
                           " :Welcome to the IRC server, " +
                           from->getNickName() + "!";
  msg.addResponse(from, welcomeMsg);
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
      msg.addResponse(it->second, msg.getRaw());
    }
  }
  return msg.getResponses();
}
