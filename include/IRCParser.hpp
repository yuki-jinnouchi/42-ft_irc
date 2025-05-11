#pragma once
#ifndef __IRC_PARSER_HPP__
#define __IRC_PARSER_HPP__

#include <map>
#include <string>
#include <vector>

#include "IRCMessage.hpp"

class IRCMessage;

class IRCParser {
 public:
  static bool parseRaw(IRCMessage& msg);

 private:
  // Orthodox Cannonical Form
  // IRCParser();
  // ~IRCParser();
  // IRCParser(const IRCParser& other);
  // IRCParser& operator=(const IRCParser& other);

  // Parsing functions
  static bool parseCrlf(IRCMessage& msg);
  static bool extractPrefix(IRCMessage& msg, std::string::size_type& pos);
  static bool extractCommand(IRCMessage& msg, std::string::size_type& pos);
  static bool extractParams(IRCMessage& msg, std::string::size_type& pos);
  static bool parsePrefix(const std::string& prefix);
  static bool parseCommand(IRCMessage& msg, const std::string& command);
};

#endif  // __IRC_PARSER_HPP__
