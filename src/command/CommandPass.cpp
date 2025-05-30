#include "command/CommandPass.hpp"

#include "IRCLogger.hpp"

/*
  @brief IRC command "PASS" handler.
  @details This class handles the "PASS" command in IRC, which is used for
           password authentication during the connection registration process.
*/

CommandPass::CommandPass(IRCServer* server) : ACommand(server, "PASS") {}

CommandPass::~CommandPass() {}

void CommandPass::execute(IRCMessage& msg) {
  // Client* from = msg.getFrom();
  // std::string password = msg.getParam(0);
  // // NOTE: ignore hop count
  // if (password.empty()) {
  //   // TODO
  //   // ResponseBuilder::addResponse({from}, ERR_NEEDMOREPARAMS, {"PASS"});
  //   DEBUG_MSG("CommandPass: Password is empty.");
  //   return;
  // }
  // if (from->getIsRegistered()) {
  //   // TODO
  //   // ResponseBuilder::addResponse({from}, ERR_ALREADYREGISTRED, {});
  //   DEBUG_MSG("CommandPass: Client is already registered.");
  //   return;
  // }
  // from->setPassword(password);
  (void)msg;
}
