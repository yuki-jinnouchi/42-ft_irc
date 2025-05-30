#include "CommandManager.hpp"

CommandManager::CommandManager(IRCServer* server) : server_(server) {
  registerCommands();
}

CommandManager::~CommandManager() {
  unregisterCommands();
}

CommandManager::CommandManager(const CommandManager& other) {
  *this = other;
}

CommandManager& CommandManager::operator=(const CommandManager& other) {
  if (this == &other) {
    return *this;
  }
  server_ = other.server_;
  return *this;
}

void CommandManager::registerCommands() {
  // commands_["CAP"] = new CommandCap(server_, "CAP");
  commands_["PASS"] = new CommandPass(server_, "PASS");
  // commands_["NICK"] = new CommandNick(server_, "NICK");
  // commands_["USER"] = new CommandUser(server_, "USER");
  // commands_["JOIN"] = new CommandJoin(server_, "JOIN");
  // commands_["PART"] = new CommandPart(server_, "PART");
  // commands_["PRIVMSG"] = new CommandPrivmsg(server_, "PRIVMSG");
  // commands_["QUIT"] = new CommandQuit(server_, "QUIT");
  // commands_["PING"] = new CommandPing(server_, "PING");
  // commands_["PONG"] = new CommandPong(server_, "PONG");
}

void CommandManager::unregisterCommands() {
  for (std::map<std::string, ACommand*>::iterator it = commands_.begin();
       it != commands_.end(); ++it) {
    delete it->second;
  }
  commands_.clear();
}

void CommandManager::execCommand(IRCMessage& msg) {
  std::string commandName = msg.getCommand();
  ACommand* command = getCommand(commandName);
  if (command)
    command->execute(msg);
  else
    ResponseBuilder::addResponse({msg.getFrom()}, ERR_UNKNOWNCOMMAND,
                                 {commandName});
}

ACommand* CommandManager::getCommand(const std::string& commandName) {
  auto it = commands_.find(commandName);
  if (it != commands_.end()) return it->second;
  return nullptr;  // Command not found
}
