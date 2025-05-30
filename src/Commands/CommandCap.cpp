#include "CommandCap.hpp"

/*
  @brief IRC command "CAP" handler.
  @details This class handles the "CAP" command in IRC, which is used for
           capability negotiation between the client and server.
  @url https://ircv3.net/specs/core/capability-negotiation-3.1.html
*/

void CommandCap::execute(IRCMessage& msg) {
  std::vector<std::string> params = msg.getParams();

  if (params.empty()) {
    msg.addResponse(msg.getFrom(), "CAP * :No parameters provided");
  } else if (params[0] == "LS") {
    commandCapList(msg);
  } else if (params[0] == "ACK") {
    commandCapAck(msg);
  } else if (params[0] == "END") {
    commandCapEnd(msg);
  } else {
    
  return;
}

  // Respond with the list of capabilities
  std::string response = "CAP * LS :multi-prefix";
  msg.addResponse(msg.getFrom(), response);
}

// Handle the "CAP LS" command
void CommandCap::commandCapList(IRCMessage& msg) {
  msg.addResponse(msg.getFrom(), "CAP * LS :multi-prefix");
}

// Handle the "CAP ACK" command
void CommandCap::commandCapAck(IRCMessage& msg) {
  std::vector<std::string> params = msg.getParams();
  if (params.size() < 2) {
    msg.addResponse(msg.getFrom(), "CAP * ACK :No capabilities to acknowledge");
    return;
  }

  // Acknowledge the requested capabilities
  std::string response = "CAP * ACK :" + params[1];
  msg.addResponse(msg.getFrom(), response);
}

// Handle the "CAP END" command
void CommandCap::commandCapEnd(IRCMessage& msg) {
  // This command indicates the end of capability negotiation
  msg.addResponse(msg.getFrom(), "CAP * END");
}
