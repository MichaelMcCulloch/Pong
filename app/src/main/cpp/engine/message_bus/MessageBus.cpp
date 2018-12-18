#include "MessageBus.h"

Message::Message(const std::string event) {
    messageEvent = event;
}

std::string Message::getEvent() {
    return messageEvent;
}

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


BusNode::BusNode(MessageBus *messageBus) {
    this->messageBus = messageBus;
    this->messageBus->addReceiver(this->getNotifyFunc());
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

void BusNode::onNotify(Message message) {
    // Do something here. Your choice. But you could do this.
    // std::cout << "Siopao! Siopao! Siopao! (Someone forgot to implement onNotify().)" << std::endl;
}
