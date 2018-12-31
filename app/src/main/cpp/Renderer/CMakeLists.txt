
add_subdirectory(glm)
add_subdirectory(glutil)

add_library(RendererLIB SHARED
        Renderer.cpp)

target_include_directories(RendererLIB PRIVATE
        ${PROJECT_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/MessageBus)

target_link_libraries(RendererLIB
        log
        glm
        glutilLIB
        MessageBusLIB)