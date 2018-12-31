#include "gtest/gtest.h"

TEST(adder, pass) {
  EXPECT_EQ(42, 40 + 2);
}

TEST(foo, fail) {
  EXPECT_EQ(42, 42 + 1);
}