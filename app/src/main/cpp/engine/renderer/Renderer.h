#pragma once

#include <iostream>
#include "EGL/egl.h"
#include "GLES3/gl32.h"
#include "glutil/glutil.h"
#include <android/log.h>

//#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
//#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

class Renderer {
public:
    Renderer() {}
    ~Renderer() {}

    void startUp(ANativeWindow *window);

    void shutDown();
private:

    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    GLuint shader;
    int32_t width;
    int32_t height;

    void term_display();

    void drawFrame();

    int init_display(ANativeWindow *window);

    int prepareOpenGL();

    int prepareShaders();
};