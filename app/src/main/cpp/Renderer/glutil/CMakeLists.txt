add_library(glutilLIB SHARED
        glutil.cpp)

target_include_directories(glutilLIB PRIVATE
        ${PROJECT_SOURCE_DIR}/)

target_link_libraries(glutilLIB
        EGL
        GLESv3
        glm
        log)