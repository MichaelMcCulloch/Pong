//
// Created by michael on 26/12/18.
//

#pragma once

#include "MessageBus.h"
#include "Renderer.h"



class Engine {

public:
    Engine(){};
    ~Engine(){};

    void startUp();
    void shutDown();

private:
    MessageBus messageBus;
    Renderer renderer;

    struct gamestate {
        int32_t x, y;
        float_t angle;
        float_t paddleA, paddleB;
        int64_t scoreA, scoreB;
    };
    struct gamestate* gamestate;
    struct engine {
        Renderer renderer;
        MessageBus messageBus;
    };
    struct engine* engine;


};

