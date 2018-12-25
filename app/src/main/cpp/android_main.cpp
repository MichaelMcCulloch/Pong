/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <initializer_list>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <cstring>
#include <cassert>


#include "common.hpp"
#include "MessageBus.h"
#include "Renderer.h"

//#define TESTING

#ifdef TESTING

#include "gtest/gtest.h"

#endif

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 */
struct engine {
    struct android_app *app;

    ASensorManager *sensorManager;
    const ASensor *accelerometerSensor;
    ASensorEventQueue *sensorEventQueue;

    int animating;

    MessageBus* messageBus;
    Renderer* renderer;

    struct saved_state state;
};


/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app *app, AInputEvent *event) {
    struct engine *engine = (struct engine *) app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;
        engine->state.x = (int32_t)AMotionEvent_getX(event, 0);
        engine->state.y = (int32_t)AMotionEvent_getY(event, 0);
        LOGI("native_activity: input: x=%d y=%d", engine->state.x, engine->state.y);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app *app, int32_t cmd) {
    struct engine *engine = (struct engine *) app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            LOGI("native_activity: received APP_CMD_SAVE_STATE");
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state *) engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            LOGI("native_activity: received APP_CMD_INIT_WINDOW");
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine->renderer->startUp(app->window,engine->messageBus);
                engine->renderer->drawFrame(engine->state.x, engine->state.y, engine->state.angle);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("native_activity: received APP_CMD_TERM_WINDOW");
            // The window is being hidden or closed, clean it up.
            engine->renderer->shutDown();
            break;
        case APP_CMD_GAINED_FOCUS:
            LOGI("native_activity: received APP_CMD_GAINED_FOCUS");
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                                               engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                                               engine->accelerometerSensor,
                                               (1000L / 60) * 1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:
            LOGI("native_activity: received APP_CMD_LOST_FOCUS");
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                                                engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine->renderer->drawFrame(engine->state.x, engine->state.y, engine->state.angle);
            break;
    }
}

/*
 * AcquireASensorManagerInstance(void)
 *    Workaround ASensorManager_getInstance() deprecation false alarm
 *    for Android-N and before, when compiling with NDK-r15
 */
#include <dlfcn.h>

ASensorManager *AcquireASensorManagerInstance(android_app *app) {

    if (!app)
        return nullptr;

    typedef ASensorManager *(*PF_GETINSTANCEFORPACKAGE)(const char *name);
    void *androidHandle = dlopen("libandroid.so", RTLD_NOW);
    PF_GETINSTANCEFORPACKAGE getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE)
            dlsym(androidHandle, "ASensorManager_getInstanceForPackage");
    if (getInstanceForPackageFunc) {
        JNIEnv *env = nullptr;
        app->activity->vm->AttachCurrentThread(&env, NULL);

        jclass android_content_Context = env->GetObjectClass(app->activity->clazz);
        jmethodID midGetPackageName = env->GetMethodID(android_content_Context,
                                                       "getPackageName",
                                                       "()Ljava/lang/String;");
        jstring packageName = (jstring) env->CallObjectMethod(app->activity->clazz,
                                                              midGetPackageName);

        const char *nativePackageName = env->GetStringUTFChars(packageName, 0);
        ASensorManager *mgr = getInstanceForPackageFunc(nativePackageName);
        env->ReleaseStringUTFChars(packageName, nativePackageName);
        app->activity->vm->DetachCurrentThread();
        if (mgr) {
            dlclose(androidHandle);
            return mgr;
        }
    }

    typedef ASensorManager *(*PF_GETINSTANCE)();
    PF_GETINSTANCE getInstanceFunc = (PF_GETINSTANCE)
            dlsym(androidHandle, "ASensorManager_getInstance");
    // by all means at this point, ASensorManager_getInstance should be available
    assert(getInstanceFunc);
    dlclose(androidHandle);

    return getInstanceFunc();
}


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {

#ifdef TESTING
    int argc = 1;

    char *name = (char *) "Testing";
    char **argv = &name;
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS()) return;

#endif
    //TODO: Initialize MessageBus, renderer


    struct engine engine;
    memset(&engine, 0, sizeof(engine));
    engine.messageBus = new MessageBus();
    engine.renderer = new Renderer();


    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = AcquireASensorManagerInstance(state);
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(
            engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(
            engine.sensorManager,
            state->looper, LOOPER_ID_USER,
            NULL, NULL);

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state *) state->savedState;
    }

    // loop waiting for stuff to do.

    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                                        (void **) &source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                                                       &event, 1) > 0) {
                        LOGV("native_activity: accelerometer: x=%f y=%f z=%f",
                             event.acceleration.x, event.acceleration.y,
                             event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine.renderer->shutDown();
                return;
            }
        }

        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine.renderer->drawFrame(engine.state.x, engine.state.y, engine.state.angle);
        }
    }
}
//END_INCLUDE(all)
