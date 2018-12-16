project(MessageBus)

#Build the library
add_library(message_bus SHARED MessageBus)

#Build the tests
add_executable(message_test MessageTest.cc)
target_link_libraries(message_test message_bus gtest)

target_link_libraries(message_bus
        android
        log)