#pragma once

#include <iostream>
#include <vector>
#include <android/log.h>


#include "glutil.h"
#include "MessageBus.h"

class Renderer : public BusNode {
public:
    Renderer() {}
    ~Renderer() {}

    void startUp(ANativeWindow*, MessageBus* m);

    void shutDown();

    int initDisplay(ANativeWindow *);

    void drawFrame(int, int, float);

private:

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    GLuint shader;
    int32_t width;
    int32_t height;

    void termDisplay();

    int prepareOpenGL();

    int prepareShaders();

    void onNotify(Message message);
};