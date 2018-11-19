//
// Created by michael on 18/11/18.
//

#ifndef PONG_MESSAGEBUS_H
#define PONG_MESSAGEBUS_H

#include "Message.h"
#include "MessageBusSubscriber.h"


#include <queue>
#include <map>
#include <vector>

class MessageBus {
public:
    MessageBus(){};
    ~MessageBus(){};
    void startUp();
    void shutDown();
    void postMessage(Message*);
    void subscribe(MessageBusSubscriber*, MessageType);

private:
    std::map<MessageType, std::vector<MessageBusSubscriber>> subscribers;
    std::queue<Message> queue;
};


#endif //PONG_MESSAGEBUS_H
