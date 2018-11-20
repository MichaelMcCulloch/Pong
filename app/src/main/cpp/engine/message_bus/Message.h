

#pragma once

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