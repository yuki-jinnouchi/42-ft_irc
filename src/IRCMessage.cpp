#include "IRCMessage.hpp"

// Orthodox Cannonical Form
IRCMessage::IRCMessage()
    : from_(NULL),
      to_(NULL),
      raw_(""),
      prefix_(""),
      command_(""),
      rpl_code_(RPL_NONE),
      err_code_(ERR_NONE),
      error_msg_(""),
      params_(std::vector<std::string>()) {}

IRCMessage::IRCMessage(Client* from, const std::string& raw)
    : from_(from),
      to_(NULL),
      raw_(raw),
      prefix_(""),
      command_(""),
      rpl_code_(RPL_NONE),
      err_code_(ERR_NONE),
      error_msg_(""),
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
  responses_ = other.responses_;
  prefix_ = other.prefix_;
  command_ = other.command_;
  rpl_code_ = other.rpl_code_;
  err_code_ = other.err_code_;
  error_msg_ = other.error_msg_;
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

const std::map<Client*, std::string>& IRCMessage::getResponses() const {
  return responses_;
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

// Setters
void IRCMessage::setRaw(const std::string& raw) {
  raw_ = raw;
}

void IRCMessage::setPrefix(const std::string& prefix) {
  prefix_ = prefix;
}

void IRCMessage::setCommand(const std::string& command) {
  command_ = command;
}

void IRCMessage::setFrom(Client* from) {
  from_ = from;
}

void IRCMessage::setTo(Client* to) {
  to_ = to;
}

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

void IRCMessage::addResponse(Client* client_to, const std::string& message) {
  responses_[client_to] = message + "\r\n";  // CRLFを追加して応答を保存
}
