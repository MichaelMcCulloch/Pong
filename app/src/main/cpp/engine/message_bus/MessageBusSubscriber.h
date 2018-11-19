//
// Created by michael on 18/11/18.
//

#ifndef PONG_MESSAGEBUSSUBSCRIBER_H
#define PONG_MESSAGEBUSSUBSCRIBER_H

#include "MessageBus.h"
#include "Message.h"

class MessageBusSubscriber {
public:
    void handleMessage(Message*);

private:
//    MessageBus* messageBus;
};


#endif //PONG_MESSAGEBUSSUBSCRIBER_H
