//
// Created by micha on 2018-12-31.
//

#pragma once

#include "../MessageBus/MessageBus.h"
#include "../logging.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;

class MockBusNode : public BusNode {
public:
    MOCK_METHOD0(update,
                 void());
private:
    void onNotify(Message message) {

        switch (message.messageType) {
            default:
                return;
        }
    }
};

TEST(messageBusTest1,
     pass) {
    MockBusNode busNode;
    EXPECT_CALL(busNode,
                update()).Times(AtLeast(1));


}
