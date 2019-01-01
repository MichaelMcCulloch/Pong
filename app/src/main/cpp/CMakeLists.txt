project(MainActivity)

cmake_minimum_required(VERSION 3.4.1)


# build native_app_glue as a static lib
add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Set Common compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Werror")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")

# MessageBus
add_library(MessageBusLIB SHARED
        MessageBus/MessageBus.cpp
        )

target_include_directories(MessageBusLIB PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
        )
target_link_libraries(MessageBusLIB
        log
        )

# glutil
add_library(glutilLIB SHARED
        Renderer/glutil/glutil.cpp)

# Renderer
add_subdirectory(Renderer/glm)
target_include_directories(glutilLIB PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
        )

target_link_libraries(glutilLIB
        EGL
        GLESv3
        glm
        log
        )

add_library(RendererLIB SHARED
        Renderer/Renderer.cpp
        )

target_include_directories(RendererLIB PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/MessageBus>
        )

target_link_libraries(RendererLIB
        log
        glm
        glutilLIB
        MessageBusLIB
        )

#Engine
add_library(EngineLIB SHARED
        Engine/Engine.cpp
        )

target_include_directories(EngineLIB PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Renderer>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/MessageBus>
        ${ANDROID_NDK}/sources/android/native_app_glue
        )

target_link_libraries(EngineLIB
        MessageBusLIB
        RendererLIB
        android
        native_app_glue
        log
        )



#add_subdirectory(tests)