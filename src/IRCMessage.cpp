
#include "IRCMessage.hpp"

IRCMessage::IRCMessage(ClientSession* from, const std::string& raw)
    : from_(from), raw_(raw) {}

IRCMessage::~IRCMessage() {}

IRCMessage::IRCMessage(const IRCMessage& other) {
  *this = other;
}

IRCMessage& IRCMessage::operator=(const IRCMessage& other) {
  if (this == &other) {
    return *this;
  }
  from_ = other.from_;
  raw_ = other.raw_;

  return *this;
}

ClientSession* IRCMessage::getFrom() const {
  return from_;
}

const std::string& IRCMessage::getRaw() const {
  return raw_;
}

const std::map<ClientSession*, std::string>& IRCMessage::getResponses() const {
  return responses_;
}

bool IRCMessage::isFromMe(const ClientSession* client) const {
  return from_ == client;
}

void IRCMessage::addResponse(ClientSession* client_to,
                             const std::string& message) {
  responses_[client_to] = message;
}
