#pragma once

#include <iostream>
#include <functional>
#include <queue>
#include <vector>

class Message {
public:
    Message(const std::string event);

    std::string getEvent();

private:
    std::string messageEvent;
};

class MessageBus {
public:
    MessageBus() {};

    ~MessageBus() {};

    void addReceiver(std::function<void(Message)> messageReceiver);

    void sendMessage(Message message);

    void notify();

private:
    std::vector<std::function<void(Message)>> receivers;
    std::queue<Message> messages;
};

class BusNode {
public:
    BusNode(MessageBus *messageBus);

    virtual void update() {}

protected:
    MessageBus *messageBus;

    std::function<void(Message)> getNotifyFunc();

    void send(Message message);

    virtual void onNotify(Message message);
};