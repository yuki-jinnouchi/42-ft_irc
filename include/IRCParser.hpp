#pragma once
#ifndef __IRC_PARSER_HPP__
#define __IRC_PARSER_HPP__

#include <map>
#include <string>
#include <vector>

#include "IRCMessage.hpp"

class IRCMessage;

// BNF
// https://www.rfc-editor.org/rfc/rfc1459#section-2.3.1
// <message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
// <prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
// <command>  ::= <letter> { <letter> } | <number> <number> <number>
// <SPACE>    ::= ' ' { ' ' }
// <params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
// <middle>   ::= <Any *non-empty* sequence of octets not including SPACE
//                or NUL or CR or LF, the first of which may not be ':'>
// <trailing> ::= <Any, possibly *empty*, sequence of octets not including
//                  NUL or CR or LF>
// <crlf>     ::= CR LF

// <target>     ::= <to> [ "," <target> ]
// <to>         ::= <channel> | <user> '@' <servername> | <nick> | <mask>
// <channel>    ::= ('#' | '&') <chstring>
// <servername> ::= <host>
// <host>       ::= see RFC 952 [DNS:4] for details on allowed hostnames
// <nick>       ::= <letter> { <letter> | <number> | <special> }
// <mask>       ::= ('#' | '$') <chstring>
// <chstring>   ::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
//                   comma (',')>

// Other parameter syntaxes are:

// <user>       ::= <nonwhite> { <nonwhite> }
// <letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
// <number>     ::= '0' ... '9'
// <special>    ::= '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
// <nonwhite>   ::= <any 8bit code except SPACE (0x20), NUL (0x0), CR
//                   (0xd), and LF (0xa)>

class IRCParser {
 public:
  static bool parseRaw(IRCMessage& msg);

  static bool isValidNickName(const std::string& nick);

 private:
  // Orthodox Cannonical Form
  IRCParser();
  ~IRCParser();
  IRCParser(const IRCParser& other);
  IRCParser& operator=(const IRCParser& other);

  // Parsing functions
  static bool extractPrefix(IRCMessage& msg, std::string::size_type& pos);
  static bool extractCommand(IRCMessage& msg, std::string::size_type& pos);
  static bool extractParams(IRCMessage& msg, std::string::size_type& pos);
  static bool parsePrefix(const std::string& prefix);
  static bool validCommand(const std::string& command);

  static bool isLetter(char c);
  static bool isNumber(char c);
  static bool isSpecial(char c);
  static bool isNonWhite(char c);
};

#endif  // __IRC_PARSER_HPP__
