project(Renderer)


#Build the library
add_library(glutil STATIC glutil)
target_link_libraries(glutil
        android
        log
        EGL
        GLESv3)

add_library(renderer SHARED Renderer)
target_include_directories(renderer PRIVATE ../message_bus/)
target_link_libraries(renderer
        android
        glutil
        message_bus
        log)