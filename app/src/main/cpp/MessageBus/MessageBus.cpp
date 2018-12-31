#include "MessageBus.h"

char* messageTypeToString(MessageType mt){
    switch(mt) {
        case _APP_CMD_SAVE_STATE:
            return (char *) "_APP_CMD_SAVE_STATE";
        case _APP_CMD_INIT_WINDOW:
            return (char *) "_APP_CMD_INIT_WINDOW";
        case _APP_CMD_TERM_WINDOW:
            return (char *) "_APP_CMD_TERM_WINDOW";
        case _APP_CMD_LOST_FOCUS:
            return (char *) "_APP_CMD_LOST_FOCUS";
        case _APP_CMD_WINDOW_RESIZED:
            return (char *) "_APP_CMD_WINDOW_RESIZED";
        case _APP_CMD_CONFIG_CHANGED:
            return (char *) "_APP_CMD_CONFIG_CHANGED";
        case _X_DISPLACEMENT:
            return (char *) "_X_DISPLACEMENT";
        case _Y_DISPLACEMENT:
            return (char *) "_Y_DISPLACEMENT";
        case _A_POSITION:
            return (char*) "_A_POSITION";
        case _B_POSITION:
            return (char*) "_B_POSITION";
        default:
            return (char *) "UNKNOWN";
    }
}

void MessageBus::addReceiver(std::function<void(Message)> messageReceiver) {
    receivers.push_back(messageReceiver);
}

void MessageBus::sendMessage(Message message) {

    LOGV("MsgBus : Received %s", messageTypeToString(message.messageType));
    messages.push(message);
}

void MessageBus::postMessage(MessageType messageType, void * data){
    struct Message msg = {
            messageType,
            data
    };
    sendMessage(msg);
}

void MessageBus::notify() {
    while (!messages.empty()) { //For each message, run whichever function the receiver has registered
        for (auto recFun = receivers.begin(); recFun != receivers.end(); recFun++) {
            (*recFun)(messages.front());
        }

        messages.pop();
    }
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
