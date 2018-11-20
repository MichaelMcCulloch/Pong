//
// Created by michael on 18/11/18.
//

#include "MessageBus.h"


void MessageBus::startUp() {
    //Create Pointers for subscriber list
    for (int i = 0; i <= MESSAGE_TYPE_MAX; ++i) {
//subscribers.[(MessageType)i] = std::vector<MessageBusSubscriber>();
    }
}

void MessageBus::shutDown() {

}

void MessageBus::postMessage(Message* message) {

}

void MessageBus::subscribe(MessageBusSubscriber, MessageType) {

}


