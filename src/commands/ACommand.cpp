#include "ACommand.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "IRCMessage.hpp"

// Constructor & Destructor
ACommand::ACommand(IRCServer* server, std::string command_name)
    : server_(server), command_name_(command_name) {}

ACommand::~ACommand() {}

// Getters
const std::string& ACommand::getCommandName() const {
  return command_name_;
}

// Member functions
void ACommand::registrate(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  if (from->getIsRegistered()) {
    reply.setResCode(ERR_ALREADYREGISTRED);
    pushResponse(reply);
  }
  if (from->getNickName().empty() || from->getUserName().empty() ||
      from->getPassword().empty())
    return;
  // TODO: if pass is wrong, kickout from server?
  if (server_->getPassword() != from->getPassword()) {
    return;
  }
  from->setIsRegistered(true);
  reply.setResCode(RPL_WELCOME);
  pushResponse(reply);
}

bool ACommand::checkIsRegistered(IRCMessage& msg) {
  Client* from = msg.getFrom();
  if (from->getIsRegistered()) return true;
  IRCMessage reply(from, from);
  reply.setResCode(ERR_NOTREGISTERED);
  pushResponse(reply);
  return false;
}

bool ACommand::checkParamNum(IRCMessage& msg, int min_params) {
  Client* from = msg.getFrom();
  int params_size = msg.getParams().size();
  if (min_params <= params_size) return true;
  IRCMessage reply(from, from);
  reply.setResCode(ERR_NEEDMOREPARAMS);
  reply.setCommand(msg.getCommand());
  pushResponse(reply);
  return false;
}

void ACommand::pushResponse(IRCMessage& reply_msg) {
  std::ostringstream oss;

  // with response code
  if (reply_msg.getResCode() != RES_NONE) {
    oss << ":" << server_->getServerName() << " " << std::setw(3)
        << std::setfill('0') << reply_msg.getResCode() << " ";
    if (reply_msg.getTo()->getNickName().empty()) {
      oss << "* ";
    } else {
      oss << reply_msg.getTo()->getNickName() << " ";
    }
    if (reply_msg.getBody().empty()) {
      oss << generateResponseMsg(reply_msg);
    } else {
      oss << reply_msg.getBody();
    }
    reply_msg.setRaw(oss.str());
    // with command
  } else if (!reply_msg.getCommand().empty()) {
    oss << ":" << reply_msg.getFrom()->getUserPrefix() << " "
        << reply_msg.getCommand();
    for (size_t i = 0; i < reply_msg.getParams().size(); ++i) {
      oss << " " << reply_msg.getParam(i);
    }
    reply_msg.setRaw(oss.str());
  }

  reply_msg.getTo()->pushSendingMsg(reply_msg.getRaw() + "\r\n");
  server_->addSendQueue(reply_msg.getTo());
}

std::string ACommand::generateResponseMsg(IRCMessage& reply_msg) {
  std::ostringstream oss;
  switch (reply_msg.getResCode()) {
    // Connection Registration
    case RPL_WELCOME:  // 001
      // <nick> :Welcome to the <network> Network, <nick>!
      // :irc.example.net 001 nick1 :Welcome to the Internet Relay Network
      // nick1!~a@localhost
      oss << reply_msg.getTo()->getNickName()
          << " :Welcome to the Internet Relay Network "
          << reply_msg.getTo()->getNickName() << "!";
      return oss.str();
      // values.push_back("nick");
      // values.push_back("network");
      // values.push_back("nick");
      // return formatResponse(responseCode, "%s :Welcome to %s, %s!", values);
      // case RPL_YOURHOST:  // 002
      //   // <nick> :Your host is <servername>, running version <version>
      //   return formatResponse(responseCode,
      //                         "%s :Your host is %s, running version %s",
      //                         values);
      // case RPL_CREATED:  // 003
      //   // <nick> :This server was created <datetime>
      //   return formatResponse(responseCode, "%s :This server was created %s",
      //                         values);
      // case RPL_MYINFO:  // 004
      //   // <nick> <servername> <version> <available umodes> <available
      //   cmodes> return formatResponse(responseCode, "%s %s %s :%s %s",
      //   values);

      // Channel Operations
    case RPL_TOPIC:  // 332
      // <channel> :<topic>
      oss << reply_msg.getParam(0) << " :" << reply_msg.getParam(1);
      return oss.str();
      // case RPL_NAMREPLY:  // 353
      //   // <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]
      //   return formatResponse(responseCode, "%s :%s", values);
      // case RPL_ENDOFNAMES:  // 366
      //   // <channel> :End of /NAMES list
      //   return formatResponse(responseCode, "%s :End of /NAMES list",
      //   values);

    // Error Codes
    case ERR_NOSUCHNICK:  // 401
      // <nick> :No such nick/channel
      if (reply_msg.getParam(0).empty()) {
        throw std::invalid_argument("ERR_NOSUCHNICK");
      }
      oss << reply_msg.getParam(0) << " :No such nick/channel";
      return oss.str();
    case ERR_NOSUCHCHANNEL:  // 403
      // <channel> :No such channel
      if (reply_msg.getParam(0).empty()) {
        throw std::invalid_argument("ERR_NOSUCHCHANNEL");
      }
      oss << reply_msg.getParam(0) << " :No such channel";
      return oss.str();
      // case ERR_CANNOTSENDTOCHAN:  // 404
      //   // <channel> :Cannot send to channel
      //   if (reply_msg.getParam(0).empty()) {
      //     throw std::invalid_argument("ERR_CANNOTSENDTOCHAN");
      //   }
      //   oss << reply_msg.getParam(0) << " :Cannot send to channel";
      //   return oss.str();
      // case ERR_TOOMANYCHANNELS:  // 405
      //   // <channel> :You have joined too many channels
      //   if (reply_msg.getParam(0).empty()) {
      //     throw std::invalid_argument("ERR_TOOMANYCHANNELS");
      //   }
      //   oss << reply_msg.getParam(0) << " :You have joined too many
      //   channels"; return oss.str();
    case ERR_NOORIGIN:  // 409
      // :No origin specified
      return ":No origin specified";
    case ERR_NORECIPIENT:  // 411
      // ":No recipient given (<command>)"
      oss << ":No recipient given (" << getCommandName() << ")";
      return oss.str();
    case ERR_NOTEXTTOSEND:  // 412
      // ":No text to send"
      return ":No text to send";
      // case ERR_UNKNOWNCOMMAND:  // 421
      //   // <command> :Unknown command
      //   if (reply_msg.getCommand().empty()) {
      //     throw std::invalid_argument("ERR_TOOMANYCHANNELS");
      //   }
      //   oss << reply_msg.getCommand() << " :Unknown command";
      //   return oss.str();
    case ERR_NONICKNAMEGIVEN:  // 431
      // :No nickname given
      return ":No nickname given";
    case ERR_ERRONEUSNICKNAME:  // 432
      // <nick> :Erroneous nickname
      if (reply_msg.getParam(0).empty()) {
        throw std::runtime_error("Nickname is empty in ERR_ERRONEUSNICKNAME");
      }
      oss << reply_msg.getParam(0) << " :Erroneous nickname";
      return oss.str();
    case ERR_NICKNAMEINUSE:  // 433
      // <nick> :Nickname is already in use
      if (reply_msg.getParam(0).empty()) {
        throw std::runtime_error("Nickname is empty in ERR_NICKNAMEINUSE");
      }
      oss << reply_msg.getParam(0) << " :Nickname already in use";
      return oss.str();
      // case ERR_USERNOTINCHANNEL:  // 441
      //   // <nick> <channel> :They aren't on that channel
      //   return formatResponse(responseCode, "%s %s :They aren't on that
      //   channel",
      //                         values);
      // case ERR_NOTONCHANNEL:  // 442
      //   // <channel> :You're not on that channel
      //   return formatResponse(responseCode, "%s :You're not on that channel",
      //                         values);
    case ERR_USERONCHANNEL:  // 443
      // <nick> <channel> :is already on channel
      if (reply_msg.getParam(0).empty()) {
        throw std::runtime_error("channel is empty in ERR_USERONCHANNEL");
      }
      oss << reply_msg.getFrom()->getNickName() << " " << reply_msg.getParam(0)
          << " :is already on channel";
      return oss.str();
    case ERR_NOTREGISTERED:  // 451
      // :You have not registered
      return ":You have not registered";
    case ERR_NEEDMOREPARAMS:  // 461
      // <command> :Not enough parameters
      oss << getCommandName() << " :Not enough parameters";
      return oss.str();
    case ERR_ALREADYREGISTRED:  // 462
      return ":You may not reregister";
      // :You may not reregister
      // return formatResponse(responseCode, ":You may not reregister", values);
      // case ERR_PASSWDMISMATCH:  // 464
      //   // :Password incorrect
      //   return formatResponse(responseCode, ":Password incorrect", values);
      // case ERR_CHANNELISFULL:  // 471
      //   // <channel> :Cannot join channel (+l)
      //   return formatResponse(responseCode, "%s :Cannot join channel (+l)",
      //                         values);
    case ERR_UNKNOWNMODE:  // 472
                           // <char> :is unknown mode char to me
      oss << reply_msg.getParam(0) << " :is unknown mode char to me";
      return oss.str();
      // case ERR_INVITEONLYCHAN:  // 473
      //   // <channel> :Cannot join channel (+i)
      //   return formatResponse(responseCode, "%s :Cannot join channel (+i)",
      //                         values);
      // case ERR_BADCHANNELKEY:  // 475
      //   // <channel> :Cannot join channel (+k)
      //   return formatResponse(responseCode, "%s :Cannot join channel (+k)",
      //                         values);
    case ERR_CHANOPRIVSNEEDED:  // 482
      // <channel> :You're not channel operator
      oss << reply_msg.getParam(0) << " :You're not channel operator";
      return oss.str();
    default:
      return "";
  }
}
