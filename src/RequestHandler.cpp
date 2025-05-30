#include "RequestHandler.hpp"

#include "command/Ping.hpp"

// Orthodox Cannonical Form
RequestHandler::RequestHandler(IRCServer* server) : server_(server) {}

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

void RequestHandler::initializeCommands(IRCServer* server_) {
  // commandMap_["CAP"] = new Cap(server_);
  // commandMap_["PASS"] = new Pass(server_);
  // commandMap_["NICK"] = new Nick(server_);
  // commandMap_["USER"] = new User(server_);
  // commandMap_["JOIN"] = new Join(server_);
  // commandMap_["PRIVMSG"] = new Privmsg(server_);
  // commandMap_["PART"] = new Part(server_);
  // commandMap_["QUIT"] = new Quit(server_);
  commandMap_["PING"] = new Ping(server_);
}

// Member functions
const std::map<Client*, std::string>& RequestHandler::handleCommand(
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
  std::string cmd = msg.getCommand();
  auto it = commandMap_.find(cmd);
  if (it != commandMap_.end()) {
    it->second->execute(msg);
  } else {
    broadCastRawMsg(msg);
  }

  return msg.getResponses();
}

// void RequestHandler::cap(IRCMessage& msg) {
//   Client* from = msg.getFrom();
//   if (msg.getParam(0).empty()) {
//     msg.pushResponse(msg.getFrom(), "461 CAP :Not enough parameters");
//   } else if (msg.getParam(0) == "LS") {  // CAP LS
//     from->setIsCapabilityNegotiating(true);
//     msg.pushResponse(msg.getFrom(), "CAP * LS :multi-prefix");
//   } else if (msg.getParam(0) == "REQ") {  // CAP REQ
//     msg.pushResponse(msg.getFrom(), "CAP * ACK :multi-prefix");
//   } else if (msg.getParam(0) == "END") {  // CAP END
//     msg.pushResponse(msg.getFrom(), "CAP END");
//     from->setIsCapabilityNegotiating(false);
//   } else {
//     msg.pushResponse(msg.getFrom(), "421 :Invalid CAP command");
//   }
// }

void RequestHandler::pass(IRCMessage& msg) {
  Client* from = msg.getFrom();
  std::string password = msg.getParam(0);
  // NOTE: ignore hop count
  if (password.empty()) {
    std::string errorMsg = "461 PASS :Not enough parameters";
    msg.pushResponse(from, errorMsg);
    return;
  }
  if (from->getIsPasswordPassed()) {
    std::string errorMsg = "462 :You may not reregister";
    msg.pushResponse(from, errorMsg);
    return;
  }
  if (password != server_->getPassword()) {
    std::string errorMsg = "464 :Password incorrect";
    msg.pushResponse(from, errorMsg);
    // TODO:パスワードが間違っている場合には、クライアントを切断したい
    return;
  }
  from->setIsPasswordPassed(true);
}

void RequestHandler::nick(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (msg.getParam(0).empty()) {
    std::string errorMsg = "431 :No nickname given";
    msg.pushResponse(from, errorMsg);
    return;
  }
  if (msg.getParam(0).size() > Client::kMaxNickNameSize) {
    std::string errorMsg = "432 :Nickname too long";
    msg.pushResponse(from, errorMsg);
    return;
  }
  if (server_->isNickTaken(msg.getParam(0))) {
    std::string errorMsg =
        "433 " + msg.getParam(0) + " :Nickname is already in use";
    msg.pushResponse(from, errorMsg);
    return;
  }
  // TODO: valid Nick characters (432 ERR_ERRONEUSNICKNAME)
  from->setNickName(msg.getParam(0));
  sendWelcome(msg);
}

void RequestHandler::user(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (msg.getParam(0).empty() || msg.getParam(3).empty()) {
    std::string errorMsg = "461 USER :Not enough parameters";
    return;
  }
  if (from->getIsRegistered()) {
    std::string errorMsg = "462 :You may not reregister";
    msg.pushResponse(from, errorMsg);
    return;
  }
  // TODO: Valid入れる？
  from->setUserName(msg.getParam(0));
  from->setRealName(msg.getParam(3));
  sendWelcome(msg);
}

void RequestHandler::join(IRCMessage& msg) {
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
    msg.pushResponse(client, errorMsg);
    return;
  }
  // TODO: now ignore invitation and password
  if (!server_->ifChannleExists(channelName))
    server_->addChannel(channelName, client);
  else
    server_->getChannel(channelName)->addMember(client);
}

// void RequestHandler::Privmsg(const IRCMessage& msg) {
//   // TODO: 追って追記
// }

// void RequestHandler::Part(const IRCMessage& msg) {
//   // TODO: 追って追記
// }

// void RequestHandler::Quit(const IRCMessage& msg) {
//   // TODO: 追って追記
// }

void RequestHandler::ping(IRCMessage& msg) {
  msg.pushResponse(msg.getFrom(), "PONG " + msg.getParam(0));
}

void RequestHandler::sendWelcome(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (from->getIsRegistered() || !from->getIsPasswordPassed() ||
      from->getNickName().empty() || from->getUserName().empty()) {
    return;
  }
  from->setIsRegistered(true);
  std::string welcomeMsg = "001 " + from->getNickName() +
                           " :Welcome to the IRC server, " +
                           from->getNickName() + "!";
  msg.pushResponse(from, welcomeMsg);
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
      msg.pushResponse(it->second, msg.getRaw());
    }
  }
  return msg.getResponses();
}
