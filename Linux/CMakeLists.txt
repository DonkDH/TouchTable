cmake_minimum_required(VERSION 2.8)
SET(CMAKE_CXX_FLAGS "-std=c++0x")
project( TouchTable )
find_package( OpenCV REQUIRED )
file(GLOB SRC_CODE
    "../Code/*.h"
    "../Code/*.cpp"
)
add_executable(TouchTable ${SRC_CODE})
target_link_libraries( TouchTable ${OpenCV_LIBS} )
