#pragma once

#include <EGL/egl.h>
#include <GLES3/gl32.h>

#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "glutil", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "glutil", __VA_ARGS__))

bool CheckGLErrors();

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const char *source);

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
