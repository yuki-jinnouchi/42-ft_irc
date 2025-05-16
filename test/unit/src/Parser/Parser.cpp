#include <gtest/gtest.h>
#include "IRCParser.hpp"  // Include your IRC Parser header

// Test basic command parsing
TEST(ParseTest, NullMessage) {
    std::string str_msg = "";
    IRCMessage msg(nullptr, str_msg);
    IRCParser::parseRaw(msg);
    // Test empty message parsing
    EXPECT_EQ(msg.getCommand(), "");
    EXPECT_EQ(msg.getPrefix(), "");
    EXPECT_EQ(msg.getParams().size(), 0);
}

TEST(ParseTest, ValidMessageWithPrefix) {
    std::string str_msg = ":server.example.com PRIVMSG #channel :Hello, world!";
    IRCMessage msg2(nullptr, str_msg);
    IRCParser::parseRaw(msg2);
    EXPECT_EQ(msg2.getPrefix(), "server.example.com");
    EXPECT_EQ(msg2.getCommand(), "PRIVMSG");
    EXPECT_EQ(msg2.getParams().size(), 2);
    EXPECT_EQ(msg2.getParam(0), "#channel");
    EXPECT_EQ(msg2.getParam(1), "Hello, world!");
}

TEST(ParseTest, ValidNumericCommand) {
    std::string str_msg = ":irc.server.net 001 nickname :Welcome to the IRC Network";
    IRCMessage msg(nullptr, str_msg);
    IRCParser::parseRaw(msg);
    EXPECT_EQ(msg.getPrefix(), "irc.server.net");
    EXPECT_EQ(msg.getCommand(), "001");
    EXPECT_EQ(msg.getParams().size(), 2);
    EXPECT_EQ(msg.getParam(0), "nickname");
    EXPECT_EQ(msg.getParam(1), "Welcome to the IRC Network");
}

// Test valid command with no trailing parameter
TEST(ParseTest, ValidCommandNoTrailing) {
    std::string str_msg = "JOIN #testchannel";
    IRCMessage msg(nullptr, str_msg);
    IRCParser::parseRaw(msg);
    EXPECT_EQ(msg.getPrefix(), "");
    EXPECT_EQ(msg.getCommand(), "JOIN");
    EXPECT_EQ(msg.getParams().size(), 1);
    EXPECT_EQ(msg.getParam(0), "#testchannel");
}

// Test valid complex prefix with user info
TEST(ParseTest, ValidComplexPrefix) {
    std::string str_msg = ":nickname!user@host.com PRIVMSG #channel :Message content";
    IRCMessage msg(nullptr, str_msg);
    IRCParser::parseRaw(msg);
    EXPECT_EQ(msg.getPrefix(), "nickname!user@host.com");
    EXPECT_EQ(msg.getCommand(), "PRIVMSG");
    EXPECT_EQ(msg.getParams().size(), 2);
    EXPECT_EQ(msg.getParam(0), "#channel");
    EXPECT_EQ(msg.getParam(1), "Message content");
}

// Test valid command with multiple space-separated parameters
TEST(ParseTest, ValidMultipleParams) {
    std::string str_msg = "MODE #channel +o user1 user2";
    IRCMessage msg(nullptr, str_msg);
    IRCParser::parseRaw(msg);
    EXPECT_EQ(msg.getPrefix(), "");
    EXPECT_EQ(msg.getCommand(), "MODE");
    EXPECT_EQ(msg.getParams().size(), 4);
    EXPECT_EQ(msg.getParam(0), "#channel");
    EXPECT_EQ(msg.getParam(1), "+o");
    EXPECT_EQ(msg.getParam(2), "user1");
    EXPECT_EQ(msg.getParam(3), "user2");
}

// Test valid simple command with no parameters
TEST(ParseTest, ValidSimpleCommand) {
    std::string str_msg = "PING";
    IRCMessage msg(nullptr, str_msg);
    IRCParser::parseRaw(msg);
    EXPECT_EQ(msg.getPrefix(), "");
    EXPECT_EQ(msg.getCommand(), "PING");
    EXPECT_EQ(msg.getParams().size(), 0);
    EXPECT_EQ(msg.getParam(0), "");

}
