//
// Created by michael on 18/11/18.
//

#include "gtest/gtest.h"
#include "MessageBus.h"

TEST(Zero, Zero) {
    EXPECT_EQ(0, 1 - 1);
}
/**
 * NOTE:
 * To run tests on device, execute the following:
 */
 //$ adb push app/build/intermediates/cmake/debug/obj/arm64-v8a/* /data/local/tmp
 //$ adb shell chmod 775 /data/local/tmp/message_test
 //$ adb shell "LD_LIBRARY_PATH=/data/local/tmp /data/local/tmp/message_test"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}