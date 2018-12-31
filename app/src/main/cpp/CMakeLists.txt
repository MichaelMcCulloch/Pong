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

#Import CMakeLists.txt from glm library
add_subdirectory(glm)



#Build the library
add_library(native-activity SHARED
        android_main.cpp
        glutil.cpp
        Engine.cpp
        MessageBus.cpp
        Renderer.cpp)

target_include_directories(native-activity PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${ANDROID_NDK}/sources/android/native_app_glue)

#Link to all created libraries
target_link_libraries(native-activity
        android
        native_app_glue
        EGL
        GLESv3
        glm
        log)

add_subdirectory(tests)