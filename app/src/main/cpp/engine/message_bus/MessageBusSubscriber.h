//
// Created by michael on 18/11/18.
//

#pragma once

#include "Message.h"
#include "MessageBus.h"

class MessageBus; //fwd declaration
class MessageBusSubscriber {
    friend class MessageBus;
public:
    void handleMessage(Message *);

private:
    std::queue<Message> messageQ;
    MessageBus* messageBus;
};
