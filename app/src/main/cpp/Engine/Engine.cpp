//
// Created by michael on 26/12/18.
//

#include "Engine.h"



/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {

#if TESTING
    int argc = 1;

    char *name = (char *) "Testing";
    char **argv = &name;
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS()) return;
#endif

    //TODO: Initialize MessageBus, renderer
    Engine *engine = new Engine();
    engine->startUp(state);
    engine->gameLoop();
    engine->shutDown();



}

/*
 * AcquireASensorManagerInstance(void)
 *    Workaround ASensorManager_getInstance() deprecation false alarm
 *    for Android-N and before, when compiling with NDK-r15
 */


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
    const int action = AMotionEvent_getAction(event);
    const int type = action & AMOTION_EVENT_ACTION_MASK;
    if (type == AMOTION_EVENT_ACTION_MOVE) {
        animating = 1;
    } else {
        return 1;
    }
    size_t count = AMotionEvent_getPointerCount(event);

    for (int i = 0; i < count; ++i) {
        const float x = AMotionEvent_getX(event, i);
        const float y = AMotionEvent_getY(event, i);
        LOGV("Motion %d, %f, %f", i, x, y);
        if (y > 2960/2) { //TODO: Remove magic number
            //A
            gamestate.paddleA = x;
            messageBus->postMessage(_A_POSITION, (void*) &gamestate.paddleA);
        } else {
            //B
            gamestate.paddleB = x;
            messageBus->postMessage(_B_POSITION, (void*) &gamestate.paddleB);
        }
    }
    return 0 ;
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
            break;
        case APP_CMD_CONFIG_CHANGED:
            messageBus->postMessage(_APP_CMD_CONFIG_CHANGED, NULL);
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




    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        gamestate = *(struct gamestate *) state->savedState;
    }

}

void Engine::gameLoop(){

    //engine subsystems cannot accept commands before they startUp();
    mApp->userData = this;
    mApp->onAppCmd = _handleCmdProxy;
    mApp->onInputEvent = _handleInputProxy;

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
