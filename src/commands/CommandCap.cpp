#include "CommandCap.hpp"

#include "IRCLogger.hpp"

/*
  @brief IRC command "CAP" handler.
  @details This class handles the "CAP" command in IRC, which is used for
           capability negotiation between the client and server.
  @url https://ircv3.net/specs/core/capability-negotiation-3.1.html
*/

void CommandCap::execute(IRCMessage& msg) {
  std::vector<std::string> params = msg.getParams();

  if (params.empty()) {
    addResponseText(msg.getFrom(), "CAP * LS :multi-prefix");
  } else if (params[0] == "LS") {
    commandCapList(msg);
  } else if (params[0] == "ACK") {
    commandCapAck(msg);
  } else if (params[0] == "END") {
    commandCapEnd(msg);
  }
}

// Handle the "CAP LS" command
void CommandCap::commandCapList(IRCMessage& msg) {
  addResponseText(msg.getFrom(), "CAP * LS :multi-prefix");
}

// Handle the "CAP ACK" command
void CommandCap::commandCapAck(IRCMessage& msg) {
  std::vector<std::string> params = msg.getParams();
  if (params.size() < 2) {
    addResponseText(msg.getFrom(), "CAP * ACK :No capabilities to acknowledge");
    return;
  }

  // Acknowledge the requested capabilities
  std::string response = "CAP * ACK :" + params[1];
  addResponseText(msg.getFrom(), response);
}

// Handle the "CAP END" command
void CommandCap::commandCapEnd(IRCMessage& msg) {
  // This command indicates the end of capability negotiation
  addResponseText(msg.getFrom(), "CAP * END");
}
