#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "glutil/glutil.h"
#include <EGL/egl.h>
#include "MessageBus.h"
#include "logging.h"


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
    float a_Displacement = 0, b_Displacement = 0;

    

    void termDisplay();

    int prepareOpenGL();

    int prepareShaders();

    void onNotify(Message message);
};