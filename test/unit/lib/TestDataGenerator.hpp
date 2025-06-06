#pragma once

#include "IRCServer.hpp"
#include "RequestHandler.hpp"

class TestDataGenerator {
 public:
  static void callCommand(RequestHandler &rh, Client *client,
                          const std::string &command);

  static void makeUserData(IRCServer &server, std::map<int, Client *> &clients,
                           RequestHandler &rh);

 private:
  TestDataGenerator();
  ~TestDataGenerator();
};
