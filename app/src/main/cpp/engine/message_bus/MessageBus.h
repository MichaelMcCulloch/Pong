//
// Created by michael on 18/11/18.
//

#pragma once

#include <queue>
#include <map>
#include <vector>
#include <string>

enum MessageType {
    Alpha,
    Beta,
    Gamma,
    Sigma,
    MESSAGE_TYPE_MAX = Sigma
};

struct Message {
    MessageType messageType;
    std::string messageContent;
};


class MessageBusSubscriber {
public:
    void handleMessage(Message *);

private:
//    MessageBus* messageBus;
};


class MessageBus {
public:
    MessageBus(){};
    ~MessageBus(){};
    void startUp();
    void shutDown();
    void postMessage(Message*);

    void subscribe(MessageBusSubscriber, MessageType);

private:
    std::queue<Message> queue;
};

