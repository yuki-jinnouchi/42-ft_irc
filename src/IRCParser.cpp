#include "IRCParser.hpp"

bool IRCParser::parseRaw(IRCMessage& msg) {
  std::string::size_type pos = 0;
  std::string raw = msg.getRaw();
  // extract prefix
  if (!extractPrefix(msg, pos)) return false;
  // extract command
  if (!extractCommand(msg, pos)) return false;
  // extract params
  if (!extractParams(msg, pos)) return false;
  return true;
}

bool IRCParser::extractPrefix(IRCMessage& msg, std::string::size_type& pos) {
  if (msg.getRaw()[pos] != ':' || pos == std::string::npos) return true;
  std::string raw = msg.getRaw();
  std::string prefix;
  std::string::size_type end = raw.find_first_of(" ", pos);
  prefix = raw.substr(1, end - 1);
  pos = raw.find_first_not_of(" ", end);
  if (!parsePrefix(prefix)) return false;
  msg.setPrefix(prefix);
  return true;
}

bool IRCParser::extractCommand(IRCMessage& msg, std::string::size_type& pos) {
  if (pos == std::string::npos) return true;
  std::string raw = msg.getRaw();
  std::string command;
  pos = raw.find_first_not_of(" ", pos);
  std::string::size_type end = raw.find_first_of(" ", pos);
  if (pos == std::string::npos) return false;
  command = raw.substr(pos, end - pos);
  pos = raw.find_first_not_of(" ", end);
  if (!validCommand(command)) return false;
  msg.setCommand(command);
  return true;
}

bool IRCParser::extractParams(IRCMessage& msg, std::string::size_type& pos) {
  if (pos == std::string::npos) return true;
  std::string raw = msg.getRaw();
  std::string param;
  std::string::size_type end;
  int paramCount = 0;
  while (pos != std::string::npos && paramCount < 15) {
    end = raw.find_first_of(" ", pos);
    if (raw[pos] == ':') {
      param = raw.substr(pos + 1);
      msg.addParam(param);
      break;
    } else {
      param = raw.substr(pos, end - pos);
    }
    pos = raw.find_first_not_of(" ", end);
    msg.addParam(param);
    paramCount++;
  }
  return true;
}

bool IRCParser::parsePrefix(const std::string& prefix) {
  if (prefix.empty()) return false;
  // TODO: prefixの解析を追加
  return true;
}

// <command>  ::= <letter> { <letter> } | <number> <number> <number>
bool IRCParser::validCommand(const std::string& command) {
  if (command.empty()) return false;
  bool all_digits = true;
  bool all_letters = true;
  for (size_t i = 0; i < command.size(); ++i) {
    if (!isLetter(command[i])) {
      all_letters = false;
    }
    if (!isNumber(command[i])) {
      all_digits = false;
    }
  }
  if (all_letters && command.size() > 0) {
    return true;  // All letters
  }
  if (all_digits && command.size() == 3) {
    return true;  // All digits and exactly 3 characters
  }
  return false;
}

bool IRCParser::isLetter(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}
bool IRCParser::isNumber(char c) {
  return ('0' <= c && c <= '9');
}
bool IRCParser::isSpecial(char c) {
  return c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' ||
         c == '^' || c == '{' || c == '}';
}
bool IRCParser::isNonWhite(char c) {
  return c != ' ' && c != '\0' && c != '\r' && c != '\n';
}
bool IRCParser::isValidNickName(const std::string& nick) {
  if (nick.empty() || nick.size() > Client::kMaxNickNameSize) {
    return false;
  }
  for (size_t i = 0; i < nick.size(); ++i) {
    char c = nick[i];
    if (i == 0 && !isLetter(c)) {
      return false;
    }
    if (i > 0 && !isLetter(c) && !isNumber(c) && !isSpecial(c)) {
      return false;
    }
  }
  return true;
}
