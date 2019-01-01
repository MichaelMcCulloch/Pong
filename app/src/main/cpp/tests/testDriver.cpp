#include "gtest/gtest.h"

#include "message_bus_test.h"
#include "renderer_test.h"
#include "engine_test.h"

TEST(adder,
     pass) {
    EXPECT_EQ(42,
              40 + 2);
}

TEST(foo,
     fail) {
    EXPECT_EQ(42,
              42 + 1);
}