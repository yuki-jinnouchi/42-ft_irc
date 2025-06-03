#pragma once
#ifndef __IRC_SIGNAL_HPP__
#define __IRC_SIGNAL_HPP__

#include <csignal>

extern volatile sig_atomic_t g_signal;

class IRCSignal {
 public:
  static void setHandler();
  static bool isShutdown();

 private:
  IRCSignal();
  ~IRCSignal();
  IRCSignal(const IRCSignal& other);
  IRCSignal& operator=(const IRCSignal& other);
};

#endif  // __IRC_SIGNAL_HPP__
