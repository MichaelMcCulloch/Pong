
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))


/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {


    //MessageBus startup
    //Subsystems startup:
        //renderer, input, etc...


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {
        //read all pending events
        int ident;
        int events;
        struct android_poll_source* source;

        //loop until all events are read
        while((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0){

            //Process this event
            if (source != NULL){
                source->process(state, source);
            }

            if (state->destroyRequested != 0) {
                //shutdown subsystems;
                return;
            }
        }

        //draw frame
    }
#pragma clang diagnostic pop



}
//END_INCLUDE(all)
