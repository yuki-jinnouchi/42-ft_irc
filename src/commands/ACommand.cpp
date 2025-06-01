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
    reply.setErrCode(ERR_ALREADYREGISTRED);
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
  reply.setRplCode(RPL_WELCOME);
  pushResponse(reply);
}

void ACommand::pushResponse(IRCMessage& reply_msg) {
  std::ostringstream oss;

  if (reply_msg.getErrCode() != ERR_NONE) {
    oss << ":" << server_->getServerName() << " " << reply_msg.getErrCode()
        << " " << reply_msg.getTo()->getNickName() << " "
        << generateErrorMsg(reply_msg);
    reply_msg.setRaw(oss.str());
  } else if (reply_msg.getRplCode() != RPL_NONE) {
    oss << ":" << server_->getServerName() << " " << std::setw(3)
        << std::setfill('0') << reply_msg.getRplCode() << " "
        << reply_msg.getTo()->getNickName() << " "
        << generateResponseMsg(reply_msg);
    reply_msg.setRaw(oss.str());
  }

  reply_msg.getTo()->pushSendingMsg(reply_msg.getRaw() + "\r\n");
  server_->addSendQueue(reply_msg.getTo());
}

std::string ACommand::generateResponseMsg(IRCMessage& reply_msg) {
  std::ostringstream oss;
  switch (reply_msg.getRplCode()) {
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
      // case RPL_TOPIC:  // 332
      //   // <channel> :<topic>
      //   return formatResponse(responseCode, "%s :%s", values);
      // case RPL_NAMREPLY:  // 353
      //   // <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]
      //   return formatResponse(responseCode, "%s :%s", values);
      // case RPL_ENDOFNAMES:  // 366
      //   // <channel> :End of /NAMES list
      //   return formatResponse(responseCode, "%s :End of /NAMES list",
      //   values);

    default:
      return "";
  }
}

std::string ACommand::generateErrorMsg(IRCMessage& reply_msg) {
  std::ostringstream oss;
  // std::vector<std::string> values;
  switch (reply_msg.getErrCode()) {
    // Error Codes
    // case ERR_NOSUCHNICK:  // 401
    //   // <nick> :No such nick/channel
    //   return formatResponse(responseCode, "%s :No such nick/channel",
    //   values);
    // case ERR_NOSUCHCHANNEL:  // 403
    //   // <channel> :No such channel
    //   return formatResponse(responseCode, "%s :No such channel", values);
    // case ERR_CANNOTSENDTOCHAN:  // 404
    //   // <channel> :Cannot send to channel
    //   return formatResponse(responseCode, "%s :Cannot send to channel",
    //   values);
    // case ERR_TOOMANYCHANNELS:  // 405
    //   // <channel> :You have joined too many channels
    //   return formatResponse(responseCode,
    //                         "%s :You have joined too many channels", values);
    case ERR_NOORIGIN:  // 409
      // :No origin specified
      return ":No origin specified";
    // case ERR_UNKNOWNCOMMAND:  // 421
    //   // <command> :Unknown command
    //   return formatResponse(responseCode, "%s :Unknown command", values);
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
    // case ERR_NICKNAMEINUSE:  // 433
    //   // <nick> :Nickname is already in use
    //   return formatResponse(responseCode, "%s :Nickname is already in use",
    //                         values);
    // case ERR_USERNOTINCHANNEL:  // 441
    //   // <nick> <channel> :They aren't on that channel
    //   return formatResponse(responseCode, "%s %s :They aren't on that
    //   channel",
    //                         values);
    // case ERR_NOTONCHANNEL:  // 442
    //   // <channel> :You're not on that channel
    //   return formatResponse(responseCode, "%s :You're not on that channel",
    //                         values);
    // case ERR_USERONCHANNEL:  // 443
    //   // <nick> <channel> :is already on channel
    //   return formatResponse(responseCode, "%s %s :is already on channel",
    //                         values);
    // case ERR_NOTREGISTERED:  // 451
    //   // :You have not registered
    //   return formatResponse(responseCode, ":You have not registered",
    //   values);
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
      // case ERR_UNKNOWNMODE:  // 472
      //   // <char> :is unknown mode char to me
      //   return formatResponse(responseCode, "%s :is unknown mode char to me
      //   ",
      //                         values);
      // case ERR_INVITEONLYCHAN:  // 473
      //   // <channel> :Cannot join channel (+i)
      //   return formatResponse(responseCode, "%s :Cannot join channel (+i)",
      //                         values);
      // case ERR_BADCHANNELKEY:  // 475
      //   // <channel> :Cannot join channel (+k)
      //   return formatResponse(responseCode, "%s :Cannot join channel (+k)",
      //                         values);
      // case ERR_CHANOPRIVSNEEDED:  // 482
      //   // <channel> :You're not channel operator
      //   return formatResponse(responseCode, "%s :You're not channel
      //   operator",
      //                         values);
    default:
      return "";
  }
}
