add_library(EngineLIB SHARED
        Engine.cpp)

target_include_directories(EngineLIB PRIVATE
        ${PROJECT_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/Renderer
        ${PROJECT_SOURCE_DIR}/MessageBus
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_link_libraries(EngineLIB
        MessageBusLIB
        RendererLIB
        android
        native_app_glue
        log)