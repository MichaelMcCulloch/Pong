#pragma once

#include <iostream>
#include <vector>
#include <memory>

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
    int32_t width, height;
    float_t a_Displacement = 0, b_Displacement = 0;

    

    void termDisplay();

    int prepareOpenGL();

    int prepareShaders();

    void onNotify(Message message);
};