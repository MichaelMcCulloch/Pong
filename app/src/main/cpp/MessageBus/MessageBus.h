#pragma once

#include <iostream>
#include <functional>
#include <queue>
#include <vector>

#include "logging.h"

enum class MessageType{
    APP_CMD_SAVE_STATE,
    APP_CMD_INIT_WINDOW,
    APP_CMD_TERM_WINDOW,
    APP_CMD_LOST_FOCUS,
    APP_CMD_WINDOW_RESIZED,
    APP_CMD_CONFIG_CHANGED,
    INPUT_RECEIVED
};

struct Message {
    Message(MessageType mt, void *payload);
    MessageType messageType;
    void getInput(float input[2]);

private:
    void * data;
};


class MessageBus {
public:
    MessageBus() = default;
    ~MessageBus() = default;

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

    virtual void onNotify(Message message) = 0;
};