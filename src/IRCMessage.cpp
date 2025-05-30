#include "IRCMessage.hpp"

// Orthodox Cannonical Form
IRCMessage::IRCMessage(Client* from, const std::string& raw)
    : from_(from),
      raw_(raw),
      prefix_(""),
      command_(""),
      params_(std::vector<std::string>()) {}

IRCMessage::~IRCMessage() {}

IRCMessage::IRCMessage(const IRCMessage& other) { *this = other; }

IRCMessage& IRCMessage::operator=(const IRCMessage& other) {
  if (this == &other) {
    return *this;
  }
  from_ = other.from_;
  raw_ = other.raw_;
  responses_ = other.responses_;
  prefix_ = other.prefix_;
  command_ = other.command_;
  // isReply_ = other.isReply_;
  params_ = other.params_;
  return *this;
}

// Getters
Client* IRCMessage::getFrom() const { return from_; }

const std::string& IRCMessage::getRaw() const { return raw_; }

const std::map<Client*, std::string>& IRCMessage::getResponses() const {
  return responses_;
}

const std::string& IRCMessage::getPrefix() const { return prefix_; }

const std::string& IRCMessage::getCommand() const { return command_; }

// bool IRCMessage::getIsReply() const {
//   return isReply_;
// }

const std::string& IRCMessage::getParam(int index) const {
  static const std::string empty = "";
  if (params_.empty())  // paramsが空の場合のガード
    return empty;
  if (index < 0 || index >= static_cast<int>(params_.size()))
    return empty;  // TODO: 例外を投げるか、エラーメッセージを返すなど
  return params_[index];
}

const std::vector<std::string>& IRCMessage::getParams() const {
  return params_;
}

// Setters
void IRCMessage::setRaw(const std::string& raw) { raw_ = raw; }

void IRCMessage::setPrefix(const std::string& prefix) { prefix_ = prefix; }

void IRCMessage::setCommand(const std::string& command) { command_ = command; }

// void IRCMessage::setIsReply(bool isReply) {
//   isReply_ = isReply;
// }

void IRCMessage::addParam(const std::string& param) {
  params_.push_back(param);
}

void IRCMessage::setParams(const std::vector<std::string>& params) {
  params_ = params;
}

// Member functions
bool IRCMessage::isFromMe(const Client* client) const {
  return from_ == client;
}
