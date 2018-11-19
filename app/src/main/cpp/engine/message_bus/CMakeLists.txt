project(MessageBus)

add_library(message_bus SHARED
        MessageBus
        MessageBusSubscriber
        Message)

target_link_libraries(message_bus
        android
        log)