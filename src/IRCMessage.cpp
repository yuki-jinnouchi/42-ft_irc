#include "IRCMessage.hpp"

#include "IRCServer.hpp"

// Orthodox Cannonical Form
IRCMessage::IRCMessage(Client* from, const std::string& raw)
    : from_(from),
      to_(NULL),
      raw_(raw),
      prefix_(""),
      command_(""),
      res_code_(RES_NONE),
      body_(""),
      params_(std::vector<std::string>()) {}

IRCMessage::IRCMessage(Client* from, Client* to)
    : from_(from),
      to_(to),
      raw_(""),
      prefix_(""),
      command_(""),
      res_code_(RES_NONE),
      body_(""),
      params_(std::vector<std::string>()) {}

IRCMessage::~IRCMessage() {}

IRCMessage::IRCMessage(const IRCMessage& other) {
  *this = other;
}

IRCMessage& IRCMessage::operator=(const IRCMessage& other) {
  if (this == &other) {
    return *this;
  }
  from_ = other.from_;
  to_ = other.to_;
  raw_ = other.raw_;
  prefix_ = other.prefix_;
  command_ = other.command_;
  res_code_ = other.res_code_;
  body_ = other.body_;
  params_ = other.params_;
  return *this;
}

// Getters
Client* IRCMessage::getFrom() const {
  return from_;
}

Client* IRCMessage::getTo() const {
  return to_;
}

const std::string& IRCMessage::getRaw() const {
  return raw_;
}

const std::string& IRCMessage::getPrefix() const {
  return prefix_;
}

const std::string& IRCMessage::getCommand() const {
  return command_;
}

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

IRCResponseCode IRCMessage::getResCode() const {
  return res_code_;
}

std::string IRCMessage::getBody() const {
  return body_;
}

// Setters
void IRCMessage::setFrom(Client* from) {
  from_ = from;
}

void IRCMessage::setTo(Client* to) {
  to_ = to;
}

void IRCMessage::setRaw(const std::string& raw) {
  if (raw.size() > IRCServer::kMaxMsgSize) {
    raw_ = raw.substr(0, IRCServer::kMaxMsgSize - 5) + "[CUT]";
    return;
  }
  raw_ = raw;
}

void IRCMessage::setPrefix(const std::string& prefix) {
  prefix_ = prefix;
}

void IRCMessage::setCommand(const std::string& command) {
  command_ = command;
}

void IRCMessage::addParam(const std::string& param) {
  params_.push_back(param);
}

void IRCMessage::setParams(const std::vector<std::string>& params) {
  params_ = params;
}

void IRCMessage::setResCode(IRCResponseCode res_code) {
  res_code_ = res_code;
}

void IRCMessage::setBody(const std::string& body) {
  body_ = body;
}

// Member functions
bool IRCMessage::isFromMe(const Client* client) const {
  return from_ == client;
}
