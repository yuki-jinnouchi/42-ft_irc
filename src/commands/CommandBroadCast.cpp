#include "CommandBroadCast.hpp"

CommandBroadCast::CommandBroadCast(IRCServer* server)
    : ACommand(server, "BROADCAST") {}

CommandBroadCast::~CommandBroadCast() {}

void CommandBroadCast::execute(IRCMessage& msg) {
  Client* from = msg.getFrom();
  IRCMessage reply(from, from);
  reply.setRaw(msg.getRaw());

  for (std::map<int, Client*>::const_iterator it =
           server_->getClients().begin();
       it != server_->getClients().end(); ++it) {
    if (msg.isFromMe(it->second)) {
      // 自分自身はスキップ
      continue;
    } else {
      // 受信したデータを他のクライアントにそのまま送信
      reply.setTo(it->second);
      pushResponse(reply);
    }
  }
}
