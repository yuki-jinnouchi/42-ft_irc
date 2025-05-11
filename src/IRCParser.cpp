#include "IRCParser.hpp"

bool IRCParser::parseRaw(IRCMessage& msg) {
  std::string::size_type pos = 0;
  std::string raw = msg.getRaw();
  // extract Crlf
  if (!parseCrlf(msg)) return false;
  // extract prefix
  if (!extractPrefix(msg, pos)) return false;
  // extract command
  if (!extractCommand(msg, pos)) return false;
  // extract params
  if (!extractParams(msg, pos)) return false;
  return true;
}

bool IRCParser::parseCrlf(IRCMessage& msg) {
  std::string raw = msg.getRaw();
  std::string::size_type pos = raw.find("\r\n");
  if (pos != raw.size() - 2)
    return false;
  msg.setRaw(raw.substr(0, pos));
  return true;
}

bool IRCParser::extractPrefix(IRCMessage& msg, std::string::size_type& pos) {
  if (msg.getRaw()[pos] != ':' || pos == std::string::npos) return true;
  std::string raw = msg.getRaw();
  std::string prefix;
  std::string::size_type end = raw.find_first_of(" ", pos);
  prefix = raw.substr(1, end - 1);
  pos = raw.find_first_not_of(" ", end);
  if (!parsePrefix(prefix))
    return false;
  msg.setPrefix(prefix);
  return true;
}

bool IRCParser::extractCommand(IRCMessage& msg, std::string::size_type& pos) {
  if (pos == std::string::npos)
    return true;
  std::string raw = msg.getRaw();
  std::string command;
  pos = raw.find_first_not_of(" ", pos);
  std::string::size_type end = raw.find_first_of(" ", pos);
  if (pos == std::string::npos)
    return false;
  command = raw.substr(pos, end - pos);
  pos = raw.find_first_not_of(" ", end);
  if (!validCommand(command))
    return false;
  msg.setCommand(command);
  return true;
}

bool IRCParser::extractParams(IRCMessage& msg, std::string::size_type& pos) {
  if (pos == std::string::npos)
    return true;
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
  if (prefix.empty())
    return false;
  // TODO: prefixの解析を追加
  return true;
}

bool IRCParser::validCommand(const std::string& command) {
  if (command.empty())
    return false;
  if (command.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != std::string::npos)
    return true;
  if (command.find_first_not_of("0123456789") != std::string::npos && command.size() == 3)
    return true;
  return false;
}
