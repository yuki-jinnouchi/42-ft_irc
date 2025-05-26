#include <iostream>

#include "IRCLogger.hpp"
#include "IRCServer.hpp"

int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      std::cout << "Usage: ircserv <port> <password>" << std::endl;
      return 1;
    }
    IRCServer server(argv[1], argv[2]);
    server.startListen();
    server.run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
