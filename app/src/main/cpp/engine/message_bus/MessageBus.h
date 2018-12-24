#pragma once

#include <iostream>
#include <functional>
#include <queue>
#include <vector>

enum MessageType{
    MT_Startup,
    MT_Shutdown,
    MT_HELLO
};
class Message {
public:
    Message(const MessageType mt);
    MessageType getEvent();

private:
    MessageType messageType;
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
    void startUp(MessageBus*);
    void shutdown();
    virtual void update() {}

protected:
    MessageBus *messageBus;

    std::function<void(Message)> getNotifyFunc();

    void send(Message message);

    virtual void onNotify(Message message);
};