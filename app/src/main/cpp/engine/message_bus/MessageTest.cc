//
// Created by michael on 18/11/18.
//

#include "gtest/gtest.h"
#include "MessageBus.h"

//Not really a test
TEST(A, B) {

    //receive from ComponentB
    class ComponentA : public BusNode {
    public:
        ComponentA(MessageBus *messageBus) : BusNode(messageBus) {}

    private:
        void onNotify(Message message) {
            std::cout << "I received: " << message.getEvent() << std::endl;
        }
    };

    //Send to ComponentA
    class ComponentB : public BusNode {
    public:
        ComponentB(MessageBus *messageBus) : BusNode(messageBus) {}

        void update() {
            Message greeting("hi");
            send(greeting);
        }

    private:
        void onNotify(Message message) {
            std::cout << "I received: " << message.getEvent() << std::endl;
        }
    };

    MessageBus messageBus;
    ComponentA compA(&messageBus);
    ComponentB compB(&messageBus);

    compB.update();
    messageBus.notify();
    compA.update();
    EXPECT_EQ(1, 1);
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