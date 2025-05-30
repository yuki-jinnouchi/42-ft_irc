#include "CommandNick.hpp"

/*
  @brief IRC command "CAP" handler.
  @details This class handles the "CAP" command in IRC, which is used for
           capability negotiation between the client and server.
  @url https://ircv3.net/specs/core/capability-negotiation-3.1.html
*/

CommandNick::CommandNick(IRCServer* server, std::string commandName)
    : ACommand(server, commandName) {}

CommandNick::~CommandNick() {}

void CommandNick::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (msg.getParam(0).empty()) {
    ResponseBuilder::addResponse({from}, ERR_NONICKNAMEGIVEN, {});
    return;
  }
  if (msg.getParam(0).size() > Client::kMaxNickNameSize) {
    ResponseBuilder::addResponse({from}, ERR_ERRONEUSNICKNAME,
                                 {from->getNickName()});
    return;
  }
  if (server_->isNickTaken(msg.getParam(0))) {
    ResponseBuilder::addResponse({from}, ERR_NICKNAMEINUSE,
                                 {from->getNickName()});
    return;
  }
  // TODO: valid Nick characters (432 ERR_ERRONEUSNICKNAME)
  from->setNickName(msg.getParam(0));
  return;
}
