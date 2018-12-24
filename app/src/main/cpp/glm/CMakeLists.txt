add_library(glm STATIC
		detail/glm.cpp)

target_include_directories(glm PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/..)