project(Renderer)

add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")
add_subdirectory(glm)

add_library(glutil STATIC glutil)
target_link_libraries(glutil
        android
        log
        EGL
        GLESv3
        glm)

add_library(renderer SHARED Renderer)
target_include_directories(renderer PRIVATE
        ../message_bus/
        glm)
target_link_libraries(renderer
        android
        glutil
        message_bus
        log
        glm)