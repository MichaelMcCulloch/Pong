#pragma once

#include <iostream>
#include <functional>
#include <queue>
#include <vector>

#include "common.hpp"

enum MessageType{
    _APP_CMD_SAVE_STATE,
    _APP_CMD_INIT_WINDOW,
    _APP_CMD_TERM_WINDOW,
    _APP_CMD_LOST_FOCUS,
    _APP_CMD_WINDOW_RESIZED,
    _APP_CMD_CONFIG_CHANGED,
    _X_DISPLACEMENT,
    _Y_DISPLACEMENT,
    _A_POSITION,
    _B_POSITION
};

char* messageTypeToString(MessageType mt);

struct Message {
    MessageType messageType;
    void * data;
};


class MessageBus {
public:
    MessageBus() {};
    ~MessageBus() {};

    void addReceiver(std::function<void(struct Message)> messageReceiver);
    void sendMessage(struct Message message);
    void postMessage(MessageType, void*);
    void notify();

private:
    std::vector<std::function<void(struct Message)>> receivers;
    std::queue<struct Message> messages;
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

    virtual void onNotify(Message message) = 0;
};