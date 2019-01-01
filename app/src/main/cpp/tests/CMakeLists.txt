# GoogleTest Integration
set(GOOGLETEST_ROOT ${ANDROID_NDK}/sources/third_party/googletest)
add_library(gtest STATIC ${GOOGLETEST_ROOT}/src/gtest_main.cc ${GOOGLETEST_ROOT}/src/gtest-all.cc)
target_include_directories(gtest PRIVATE ${GOOGLETEST_ROOT})
target_include_directories(gtest PUBLIC ${GOOGLETEST_ROOT}/include)

# GoogleMock Integration
set(GOOGLEMOCK_ROOT ${ANDROID_NDK}/sources/third_party/googlemock)
add_library(gmock STATIC ${GOOGLEMOCK_ROOT}/src/gmock_main.cc ${GOOGLEMOCK_ROOT}/src/gmock-all.cc)
target_include_directories(gmock PRIVATE
        ${GOOGLEMOCK_ROOT}
        ${GOOGLETEST_ROOT}
        )
target_include_directories(gmock PUBLIC
        ${GOOGLEMOCK_ROOT}/include
        ${GOOGLETEST_ROOT}/include
        )
target_link_libraries(gmock
        gtest)

add_library(nativehelper
        INTERFACE
        )

target_include_directories(nativehelper INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

add_library(gtestjni STATIC
        gtest_wrapper.cpp
        )

target_link_libraries(gtestjni
        PRIVATE
        gtest
        gmock
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
        gmock
        gtestjni
        )