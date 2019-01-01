# GoogleTest Integration
set(GOOGLETEST_ROOT ${ANDROID_NDK}/sources/third_party/googletest)
add_library(gtest STATIC ${GOOGLETEST_ROOT}/src/gtest_main.cc ${GOOGLETEST_ROOT}/src/gtest-all.cc)
target_include_directories(gtest PRIVATE ${GOOGLETEST_ROOT})
target_include_directories(gtest PUBLIC ${GOOGLETEST_ROOT}/include)


add_library(nativehelper
        INTERFACE
        )

target_include_directories(nativehelper
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

add_library(gtestjni
        STATIC
        gtest_wrapper.cpp
        )

target_link_libraries(gtestjni
        PRIVATE
        gtest
        nativehelper
        PUBLIC
        -uJava_com_michael_pong_GtestRunner_nInitialize
        -uJava_com_michael_pong_GtestRunner_nRun
        )

add_library(testDriver
        SHARED
        testDriver.cpp
        )

target_include_directories(testDriver PRIVATE
        ${CMAKE_SOURCE_DIR}/
        ${CMAKE_SOURCE_DIR}/MessageBus/
        ${CMAKE_SOURCE_DIR}/Renderer/
        ${CMAKE_SOURCE_DIR}/Engine/
        ${ANDROID_NDK}/sources/android/native_app_glue
        )


target_link_libraries(testDriver
        PRIVATE
        RendererLIB
        MessageBusLIB
        EngineLIB
        gtest
        gtestjni
        )