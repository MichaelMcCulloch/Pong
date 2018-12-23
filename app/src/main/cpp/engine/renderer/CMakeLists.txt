project(Renderer)


#Build the library
add_library(renderer SHARED Renderer)


add_subdirectory(glutil)
target_include_directories(renderer PRIVATE glutil)
#Build the tests
#add_executable(render_test RendererTest.cc)
#target_link_libraries(render_test renderer gtest)

target_link_libraries(renderer
        android
        glutil
        log)