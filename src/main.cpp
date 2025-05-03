#include <iostream>
#include "IRCServer.hpp"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
    return 1;
  }
  IRCServer server(argv[1], argv[2]);
  server.startListen();
  server.run();
  return 0;
}
