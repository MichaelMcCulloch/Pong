#pragma once

#include <iostream>
#include <vector>

#include "glutil.h"
#include "MessageBus.h"

class Renderer : public BusNode {
public:
    Renderer() {}
    ~Renderer() {}

    void startUp(ANativeWindow*, MessageBus* m);

    void shutDown();

    int initDisplay(ANativeWindow *);

    void drawFrame();

private:

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    GLuint shader;
    int32_t width;
    int32_t height;

    int xDisplacement = 0, yDisplacement = 0;

    void termDisplay();

    int prepareOpenGL();

    int prepareShaders();

    void onNotify(Message message);
};