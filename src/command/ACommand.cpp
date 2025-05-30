
#include "ACommand.hpp"

#include <sstream>

ACommand::ACommand(IRCServer* server) : server_(server) {}
ACommand::~ACommand() {}

std::string ACommand::generateReply(const IRCMessage& orig,
                                    const IRCMessage& reply) const {
  std::stringstream ss;
  if (reply.getErrorCode()) {
    // エラーメッセージの生成
    ss << ":" << server_->getName() << " " << reply.getErrorCode() << " "
       << orig.getFrom()->getNickName() << " :" << reply.getErrorMsg();
    return ss.str();
  }
  // 通常の返信メッセージの生成
  ss << ":" << server_->getName() << " " << reply.getRplCode() << " "
     << orig.getFrom()->getNickName() << " :" << reply.getRaw();
  return ss.str();
}
