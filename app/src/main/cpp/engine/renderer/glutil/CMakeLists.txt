project(glutil)

add_library(glutil STATIC glutil)

target_link_libraries(glutil
        EGL
        GLESv3
        log)