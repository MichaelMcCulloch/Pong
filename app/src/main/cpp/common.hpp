//
// Created by michael on 25/12/18.
//

#pragma once



extern "C" {
    #include <EGL/egl.h>
    #include <GLES3/gl32.h>
    #include <jni.h>
    #include <errno.h>
    #include <android/sensor.h>
    #include <android/log.h>
    #include <android_native_app_glue.h>
    #include <unistd.h>
    #include <stdlib.h>

}

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define DEBUG_TAG "PONG: "
#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, DEBUG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, DEBUG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, __VA_ARGS__))
#define ABORT_GAME { LOGE("*** GAME ABORTING."); *((volatile char*)0) = 'a'; }
#define DEBUG_BLIP LOGD("[ BLIP ]: %s:%d", __FILE__, __LINE__)