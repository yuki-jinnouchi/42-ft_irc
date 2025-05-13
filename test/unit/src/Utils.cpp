#include "Utils.hpp"
#include <gtest/gtest.h>

TEST(UtilsTest, split) {
  // ------- delimiter = "\r\n" -------------
  {
    std::vector<std::string> ret =
        Utils::split("hello world\r\nabc\r\n123", "\r\n");

    EXPECT_EQ(3, ret.size());
    EXPECT_EQ("hello world", ret[0]);
    EXPECT_EQ("abc", ret[1]);
    EXPECT_EQ("123", ret[2]);
  }
  {
    std::vector<std::string> ret = Utils::split(
        "\r\n\r\nhello world\r\n\r\n\r\nabc\r\n\r\n123\r\n\r\n", "\r\n");

    EXPECT_EQ(3, ret.size());
    EXPECT_EQ("hello world", ret[0]);
    EXPECT_EQ("abc", ret[1]);
    EXPECT_EQ("123", ret[2]);
  }
  {
    std::vector<std::string> ret =
        Utils::split("hello world\rabc\n123\r\n", "\r\n");

    EXPECT_EQ(1, ret.size());
    EXPECT_EQ("hello world\rabc\n123", ret[0]);
  }
  // ------- delimiter = " " -------------
  {
    std::vector<std::string> ret = Utils::split("hello world abc 123", " ");

    EXPECT_EQ(4, ret.size());
    EXPECT_EQ("hello", ret[0]);
    EXPECT_EQ("world", ret[1]);
    EXPECT_EQ("abc", ret[2]);
    EXPECT_EQ("123", ret[3]);
  }
  {
    std::vector<std::string> ret =
        Utils::split("        hello world\r\n        abc 123           ", " ");

    EXPECT_EQ(4, ret.size());
    EXPECT_EQ("hello", ret[0]);
    EXPECT_EQ("world\r\n", ret[1]);
    EXPECT_EQ("abc", ret[2]);
    EXPECT_EQ("123", ret[3]);
  }
}

TEST(UtilsTest, endsWith) {
  // ------- ture -------
  {
    bool ret = Utils::endsWith("hello world", "world");
    EXPECT_TRUE(ret);
  }
  {
    bool ret = Utils::endsWith("hello world", "d");
    EXPECT_TRUE(ret);
  }
  {
    bool ret = Utils::endsWith("hello\r\nworld\r\n", "\r\n");
    EXPECT_TRUE(ret);
  }
  // ------- false -------
  {
    bool ret = Utils::endsWith("hello world ", "d");
    EXPECT_FALSE(ret);
  }
  {
    bool ret = Utils::endsWith("hello\r\nworld\n", "\r\n");
    EXPECT_FALSE(ret);
  }
}
