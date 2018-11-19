project(MessageBus)


add_library(message_bus
        MessageBusSubscriber
        Message
        MessageTest
        MessageBus)

target_link_libraries(message_bus
        android
        log)