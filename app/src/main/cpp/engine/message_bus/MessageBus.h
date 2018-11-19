//
// Created by michael on 18/11/18.
//

#ifndef PONG_MESSAGEBUS_H
#define PONG_MESSAGEBUS_H

#include "Message.h"
#include "MessageBusSubscriber.h"
class MessageBus {
public:
    MessageBus(){};
    ~MessageBus(){};
    void startUp();
    void shutDown();

};


#endif //PONG_MESSAGEBUS_H
