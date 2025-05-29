#include <iostream>

#include "IRCServer.hpp"
#include "IRCSignal.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ircserv <port> <password>" << std::endl;
    return 1;
  }
  try {
    IRCSignal::setHandler();
    IRCServer server(argv[1], argv[2]);
    server.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
