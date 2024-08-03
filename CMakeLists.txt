project(UARTReceiver)

find_library(TRACKER_LIB trackerinterface .)

add_executable(UARTReceiver UARTReceiver.h UARTReceiver.cpp main.cpp)

target_link_libraries(UARTReceiver ${TRACKER_LIB})
