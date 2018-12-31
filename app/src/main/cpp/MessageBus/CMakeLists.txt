add_library(MessageBusLIB SHARED
        MessageBus.cpp)

target_include_directories(MessageBusLIB PRIVATE
        ${PROJECT_SOURCE_DIR}/)

target_link_libraries(MessageBusLIB
        log)