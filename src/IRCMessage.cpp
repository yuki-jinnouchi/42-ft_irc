
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

const ClientSession* IRCMessage::getFrom() const {
  return from_;
}

const std::string& IRCMessage::getRaw() const {
  return raw_;
}
