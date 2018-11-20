//
// Created by michael on 18/11/18.
//

#include "MessageBus.h"


void MessageBus::startUp() {
    //Create Pointers for subscriber list
    for (int i = 0; i <= MESSAGE_TYPE_MAX; ++i) {
        subscribers[(MessageType) i] = std::vector<MessageBusSubscriber>();
    }
}

void MessageBus::shutDown() {
    free(&subscribers);
    free(&queue);
}

void MessageBus::postMessage(Message &message) {
    queue.push(message);
    //TODO: Break these up
    for (MessageBusSubscriber s :subscribers[message.messageType]) {
        s.messageQ.push(message);
    }
}

/**
 * Add caller to message list;
 */
void MessageBus::subscribe(MessageBusSubscriber &subscriber, MessageType messageType) {
    subscribers[messageType].push_back(subscriber);
}


