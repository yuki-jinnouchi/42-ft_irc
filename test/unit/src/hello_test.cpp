#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

// bad test
TEST(HelloTest2, BasicAssertions2) {
  // Expect equality.
  // EXPECT_EQ(1, 2);
  EXPECT_EQ(1, 1);
}
