#pragma once
#ifndef __IRC_SIGNAL_HPP__
#define __IRC_SIGNAL_HPP__

class IRCSignal {
 public:
  static void setHandler();

 private:
  IRCSignal();
  ~IRCSignal();
  IRCSignal(const IRCSignal& other);
  IRCSignal& operator=(const IRCSignal& other);
};

#endif  // __IRC_SIGNAL_HPP__
