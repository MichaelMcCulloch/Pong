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
#include "Engine.h"

#define TESTING 0

#if TESTING
    #include "gtest/gtest.h"
#endif


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
//END_INCLUDE(all)
