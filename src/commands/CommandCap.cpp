#include "CommandCap.hpp"

#include "IRCLogger.hpp"

/*
  @brief IRC command "CAP" handler.
  @details This class handles the "CAP" command in IRC, which is used for
           capability negotiation between the client and server.
  @url https://ircv3.net/specs/core/capability-negotiation-3.1.html
*/

CommandCap::CommandCap(IRCServer* server) : ACommand(server, "CAP") {}
CommandCap::~CommandCap() {}

void CommandCap::execute(IRCMessage& msg) {
  std::vector<std::string> params = msg.getParams();
  IRCMessage reply(msg.getFrom(), msg.getFrom());

  if (params.empty()) {
    commandCapList(reply);
  } else if (params[0] == "LS") {
    commandCapList(reply);
  } else if (params[0] == "ACK") {
    commandCapAck(msg, reply);
  } else if (params[0] == "END") {
    commandCapEnd(reply);
  }
}

// Handle the "CAP LS" command
void CommandCap::commandCapList(IRCMessage& reply) {
  reply.setRaw("CAP * LS :multi-prefix");
  pushResponse(reply);
}

// Handle the "CAP ACK" command
void CommandCap::commandCapAck(IRCMessage& msg, IRCMessage& reply) {
  std::vector<std::string> params = msg.getParams();
  if (params.size() < 2) {
    reply.setRaw("CAP * ACK :No capabilities to acknowledge");
    pushResponse(reply);
    return;
  }

  // Acknowledge the requested capabilities
  reply.setRaw("CAP * ACK :" + params[1]);
  pushResponse(reply);
}

// Handle the "CAP END" command
void CommandCap::commandCapEnd(IRCMessage& reply) {
  // This command indicates the end of capability negotiation
  reply.setRaw("CAP * END");
  pushResponse(reply);
}
