//
// Created by michael on 18/11/18.
//

#pragma once

#include <queue>
#include <map>
#include <vector>
#include <string>

#include "Message.h"
#include "MessageBusSubscriber.h"

class MessageBusSubscriber; //fwd declaration
class MessageBus {
public:
    MessageBus(){};
    ~MessageBus(){};
    void startUp();
    void shutDown();
    void postMessage(Message &);
    void subscribe(MessageBusSubscriber &, MessageType);

private:
    std::map<MessageType, std::vector<MessageBusSubscriber>> subscribers;
    std::queue<Message> queue;
};

