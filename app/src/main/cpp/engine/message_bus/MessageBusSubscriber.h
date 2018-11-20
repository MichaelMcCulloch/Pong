//
// Created by michael on 18/11/18.
//

#pragma once

#include "Message.h"
#include "MessageBus.h"

class MessageBus;

class MessageBusSubscriber {
public:
    void handleMessage(Message *);

private:
    //MessageBus* messageBus;
};
