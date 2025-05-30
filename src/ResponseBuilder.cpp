#include "ResponseBuilder.hpp"

static std::string addResponse(std::vector<Client*> Clients,
                               ResponseCode responseCode,
                               const std::vector<std::string>& values) {
  std::string response = generateResponse(responseCode, values);
  for (std::vector<Client*>::iterator it = Clients.begin(); it != Clients.end();
       ++it) {
    Client* client = *it;
  }
}

static std::string formatResponse(
    int const responseCode, std::string const& fromat,
    const std::vector<std::string>& const values) {
  char codeStr[4];
  sprintf(codeStr, "%03d", responseCode);
  std::string response = std::string(codeStr) + " ";
  std::string formattedString = fromat;
  size_t pos = 0;
  size_t valueIndex = 0;
  while ((pos = formattedString.find("%s", pos)) != std::string::npos &&
         valueIndex < values.size()) {
    formattedString.replace(pos, 2, values[valueIndex]);
    pos += values[valueIndex].length();
    valueIndex++;
  }
  response += formattedString;
  return response;
}

static std::string generateResponse(int responseCode,
                                    const std::vector<std::string>& values) {
  char codeStr[4];
  sprintf(codeStr, "%03d", responseCode);

  switch (responseCode) {
    // Connection Registration
    case RPL_WELCOME:  // 001
      // <nick> :Welcome to the <network> Network, <nick>!
      return formatResponse(responseCode, "%s :Welcome to %s, %s!",
                            values);
    case RPL_YOURHOST:  // 002
      // <nick> :Your host is <servername>, running version <version>
      return formatResponse(responseCode,
                            "%s :Your host is %s, running version %s", values);
    case RPL_CREATED:  // 003
      // <nick> :This server was created <datetime>
      return formatResponse(responseCode, "%s :This server was created %s",
                            values);
    case RPL_MYINFO:  // 004
      // <nick> <servername> <version> <available umodes> <available cmodes>
      return formatResponse(responseCode, "%s %s %s :%s %s", values);

    // Channel Operations
    case RPL_TOPIC:  // 332
      // <channel> :<topic>
      return formatResponse(responseCode, "%s :%s", values);
    case RPL_NAMREPLY:  // 353
      // <channel> :[[@|+]<nick> [[@|+]<nick> [...]]]
      return formatResponse(responseCode, "%s :%s", values);
    case RPL_ENDOFNAMES:  // 366
      // <channel> :End of /NAMES list
      return formatResponse(responseCode, "%s :End of /NAMES list", values);

    // Error Codes
    case ERR_NOSUCHNICK:  // 401
      // <nick> :No such nick/channel
      return formatResponse(responseCode, "%s :No such nick/channel", values);
    case ERR_NOSUCHCHANNEL:  // 403
      // <channel> :No such channel
      return formatResponse(responseCode, "%s :No such channel", values);
    case ERR_CANNOTSENDTOCHAN:  // 404
      // <channel> :Cannot send to channel
      return formatResponse(responseCode, "%s :Cannot send to channel", values);
    case ERR_TOOMANYCHANNELS:  // 405
      // <channel> :You have joined too many channels
      return formatResponse(responseCode,
                            "%s :You have joined too many channels", values);
    case ERR_UNKNOWNCOMMAND:  // 421
      // <command> :Unknown command
      return formatResponse(responseCode, "%s :Unknown command", values);
    case ERR_NONICKNAMEGIVEN:  // 431
      // :No nickname given
      return formatResponse(responseCode, ":No nickname given", values);
    case ERR_ERRONEUSNICKNAME:  // 432
      // <nick> :Erroneous nickname
      return formatResponse(responseCode, "%s :Erroneous nickname", values);
    case ERR_NICKNAMEINUSE:  // 433
      // <nick> :Nickname is already in use
      return formatResponse(responseCode, "%s :Nickname is already in use",
                            values);
    case ERR_USERNOTINCHANNEL:  // 441
      // <nick> <channel> :They aren't on that channel
      return formatResponse(responseCode, "%s %s :They aren't on that channel",
                            values);
    case ERR_NOTONCHANNEL:  // 442
      // <channel> :You're not on that channel
      return formatResponse(responseCode, "%s :You're not on that channel",
                            values);
    case ERR_USERONCHANNEL:  // 443
      // <nick> <channel> :is already on channel
      return formatResponse(responseCode, "%s %s :is already on channel",
                            values);
    case ERR_NOTREGISTERED:  // 451
      // :You have not registered
      return formatResponse(responseCode, ":You have not registered", values);
    case ERR_NEEDMOREPARAMS:  // 461
      // <command> :Not enough parameters
      return formatResponse(responseCode, "%s :Not enough parameters", values);
    case ERR_ALREADYREGISTRED:  // 462
      // :You may not reregister
      return formatResponse(responseCode, ":You may not reregister", values);
    case ERR_PASSWDMISMATCH:  // 464
      // :Password incorrect
      return formatResponse(responseCode, ":Password incorrect", values);
    case ERR_CHANNELISFULL:  // 471
      // <channel> :Cannot join channel (+l)
      return formatResponse(responseCode, "%s :Cannot join channel (+l)",
                            values);
    case ERR_UNKNOWNMODE:  // 472
      // <char> :is unknown mode char to me
      return formatResponse(responseCode, "%s :is unknown mode char to me",
                            values);
    case ERR_INVITEONLYCHAN:  // 473
      // <channel> :Cannot join channel (+i)
      return formatResponse(responseCode, "%s :Cannot join channel (+i)",
                            values);
    case ERR_BADCHANNELKEY:  // 475
      // <channel> :Cannot join channel (+k)
      return formatResponse(responseCode, "%s :Cannot join channel (+k)",
                            values);
    case ERR_CHANOPRIVSNEEDED:  // 482
      // <channel> :You're not channel operator
      return formatResponse(responseCode, "%s :You're not channel operator",
                            values);
    default:
      return "";
  }
}
