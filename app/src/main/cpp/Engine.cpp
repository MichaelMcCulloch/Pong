//
// Created by michael on 26/12/18.
//

#include "Engine.h"


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

static void _handleCmdProxy(struct android_app *app, int32_t cmd){
    Engine* engine = (Engine*) app->userData;
    engine->handleCommand(cmd);
}

static int _handleInputProxy(struct android_app *app, AInputEvent* event){
    Engine* engine = (Engine*) app->userData;
    return engine->handleEvent(event);
}

/**
 * Process the next input event.
 */
int32_t Engine::handleEvent(AInputEvent *event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        animating = 1;
        gamestate.x = (int32_t)AMotionEvent_getX(event, 0);
        gamestate.y = (int32_t)AMotionEvent_getY(event, 0);

        messageBus->postMessage(_X_DISPLACEMENT, (void *) &gamestate.x);
        messageBus->postMessage(_Y_DISPLACEMENT, (void *) &gamestate.y);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
void Engine::handleCommand(int32_t cmd) {
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            messageBus->postMessage(_APP_CMD_SAVE_STATE, NULL);
            // The system has asked us to save our current state.  Do so.
            mApp->savedState = malloc(sizeof(struct gamestate));
            *(struct gamestate *) mApp->savedState = gamestate;
            mApp->savedStateSize = sizeof(struct gamestate);
            break;
        case APP_CMD_INIT_WINDOW:
            messageBus->postMessage(_APP_CMD_INIT_WINDOW,  NULL);
            // The window is being shown, get it ready.
            if (mApp->window != NULL) {
                renderer->startUp(mApp->window,messageBus);
                renderer->drawFrame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            messageBus->postMessage(_APP_CMD_TERM_WINDOW, NULL);
            // The window is being hidden or closed, clean it up.
            renderer->shutDown();
            break;
        case APP_CMD_LOST_FOCUS:
            messageBus->postMessage(_APP_CMD_LOST_FOCUS, NULL);
            animating = 0;
            renderer->drawFrame();
            break;
        case APP_CMD_WINDOW_RESIZED:
            messageBus->postMessage(_APP_CMD_WINDOW_RESIZED, NULL);
        case APP_CMD_CONFIG_CHANGED:
            messageBus->postMessage(_APP_CMD_CONFIG_CHANGED, NULL);
            // Window was resized or some other configuration changed.
            // Note: we don't handle this event because we check the surface dimensions
            // every frame, so that's how we know it was resized. If you are NOT doing that,
            // then TODO: you need to handle this event!
            break;
        default:
            LOGV("Engine: (unused command).");
            break;
    }
}

void Engine::startUp(struct android_app * state) {
    mApp = state;
    memset(&gamestate, 0, sizeof(struct gamestate));

    messageBus = new MessageBus();
    renderer = new Renderer();

    mApp->userData = this;
    state->onAppCmd = _handleCmdProxy;
    state->onInputEvent = _handleInputProxy;


    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        gamestate = *(struct gamestate *) state->savedState;
    }

}

void Engine::gameLoop(){


    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident = ALooper_pollAll(animating ? 0 : -1, NULL, &events,
                                        (void **) &source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(mApp, source);
            }

            // Check if we are exiting.
            if (mApp->destroyRequested != 0) {
                renderer->shutDown();
                return;
            }
        }

        if (animating) {
            // Done with events; draw next animation frame.
            gamestate.angle += .01f;
            if (gamestate.angle > 1) {
                gamestate.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            messageBus->notify();
            renderer->drawFrame();
        }
    }
}

void Engine::shutDown() {
    renderer->shutDown();
    free(&gamestate);
}
