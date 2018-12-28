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

    void startUp(android_app *state);
    void shutDown();
    void gameLoop();

    int32_t handleEvent(AInputEvent *event);
    void handleCommand(int32_t cmd);

private:
    bool animating;
    struct android_app* mApp;
    Renderer *renderer;

    MessageBus *messageBus;
    struct gamestate {
        int32_t x, y;
        float_t angle;
        float_t paddleA, paddleB;
        int64_t scoreA, scoreB;
    };
    struct gamestate gamestate;
};

