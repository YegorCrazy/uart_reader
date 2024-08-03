project(GPSBreaker)

find_library(TRACKER_LIB trackerinterface .)

add_executable(GPSBreaker main.cpp)

target_link_libraries(GPSBreaker ${TRACKER_LIB})
