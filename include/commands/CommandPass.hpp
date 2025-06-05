#pragma once
#ifndef __COMMAND_PASS_HPP__
#define __COMMAND_PASS_HPP__

#include "ACommand.hpp"

/**
 * @brief IRC command "PASS" handler.
 * @details This class handles the "PASS" command in IRC, which is used for
 *          password authentication during the connection registration process.
 * @note
    Command: PASS
    Parameters: <password>

    The PASS command is used to set a 'connection password'.  The
    password can and must be set before any attempt to register the
    connection is made.  Currently this requires that clients send a PASS
    command before sending the NICK/USER combination and servers *must*
    send a PASS command before any SERVER command.  The password supplied
    must match the one contained in the C/N lines (for servers) or I
    lines (for clients).  It is possible to send multiple PASS commands
    before registering but only the last one sent is used for
    verification and it may not be changed once registered.  Numeric

    Replies:
            ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
 * @cite https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.1
 */

class CommandPass : public ACommand {
 public:
  // Orthodox Canonical Form
  CommandPass(IRCServer* server);
  ~CommandPass();

  // Member functions
  void execute(IRCMessage& msg);

 private:
  CommandPass();
  CommandPass(const CommandPass& other);
  CommandPass& operator=(const CommandPass& other);
};

#endif  // __COMMAND_PASS_HPP__
