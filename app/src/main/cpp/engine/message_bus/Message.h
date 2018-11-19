//
// Created by michael on 18/11/18.
//

#ifndef PONG_MESSAGE_H
#define PONG_MESSAGE_H

#include <string>
enum MessageType {
    Diamonds,
    Hearts,
    Clubs,
    Spades };

struct Message {
    MessageType messageType;
    std::string messageContent;
};


#endif //PONG_MESSAGE_H
