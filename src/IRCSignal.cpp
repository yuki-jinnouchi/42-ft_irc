#include "IRCSignal.hpp"

#include <csignal>

#include "IRCLogger.hpp"

volatile sig_atomic_t g_signal = 0;

#ifdef DEBUG
static void debug_handler(int sig) {
  DEBUG_MSG("signal: " << sig);
}
#endif

static void shutdown_handler(int sig) {
  DEBUG_MSG("signal: " << sig);
  g_signal = sig;
}

static void trap_signal(int sig, sighandler_t handler) {
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESTART;  // システムコールが中断された場合、再実行
  if (sigaction(sig, &act, NULL) < 0) {
    throw std::runtime_error("sigaction failed");
  }
}

void IRCSignal::setHandler() {
#ifdef DEBUG
  trap_signal(SIGPIPE, debug_handler);
#else
  trap_signal(SIGPIPE, SIG_IGN);  // SIGPIPEは無視
#endif  // DEBUG
  trap_signal(SIGINT, shutdown_handler);
}

bool IRCSignal::isShutdown() {
  return (g_signal == SIGINT);
}
