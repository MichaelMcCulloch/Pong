project(MainActivity)


# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(native-activity SHARED native-lib.cpp)

target_include_directories(native-activity
        PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

#Build the library

foreach(TARGET
        message_bus
        renderer)
    add_subdirectory(engine/${TARGET}) #Build target Library
    target_include_directories(native-activity PRIVATE engine/${TARGET}) #Include it
endforeach()


#Link to all created libraries
target_link_libraries(native-activity
        android
        message_bus
        renderer
        native_app_glue
        gtest
        log)