#include "MessageBus.h"


void MessageBus::addReceiver(std::function<void(Message)> messageReceiver) {
    receivers.push_back(messageReceiver);
}

void MessageBus::sendMessage(Message message) {

    messages.push(message);
}

void MessageBus::notify() {
    while (!messages.empty()) { //For each message, run whichever function the receiver has registered
        for (auto recFun = receivers.begin(); recFun != receivers.end(); recFun++) {
            (*recFun)(messages.front());
        }

        messages.pop();
    }
}

Message::Message(MessageType mt, void *payload) : messageType(mt){
    switch (messageType) {
        case MessageType::APP_CMD_SAVE_STATE: {

        }
        break;
        case MessageType::APP_CMD_INIT_WINDOW: {

        }
        break;
        case MessageType::APP_CMD_TERM_WINDOW: {

        }
        break;
        case MessageType::APP_CMD_LOST_FOCUS: {

        }
        break;
        case MessageType::APP_CMD_WINDOW_RESIZED: {

        }
        break;
        case MessageType::APP_CMD_CONFIG_CHANGED: {

        }
        break;
        case MessageType::INPUT_RECEIVED: {
            //two floating point numbers
            size_t size = sizeof(float)*2;
            data = malloc(size);
            memcpy(data, payload, size);
        }
        break;
        default:break;
    }
}

void Message::getInput(float input[2]) {

    memcpy(input, data, sizeof(float)*2);
}

std::function<void(Message)> BusNode::getNotifyFunc() {
    //lambda function which takes a message and calls the class onNotify Function with it
    auto messageListener = [=](Message message) -> void {
        this->onNotify(message);
    };
    return messageListener;
}

void BusNode::send(Message message) {
    messageBus->sendMessage(message);
}



void BusNode::startUp(MessageBus *messageBus) {
    this->messageBus = messageBus;
    this->messageBus->addReceiver(this->getNotifyFunc());
}

void BusNode::shutdown() {

}


