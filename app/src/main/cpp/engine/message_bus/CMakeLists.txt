project(MessageBus)

#Build the library
add_library(message_bus SHARED MessageBus)

#Build the tests

target_link_libraries(message_bus
        android
        log)