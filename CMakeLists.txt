cmake_minimum_required(VERSION 3.23)

project(MyProjectName)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(receiver src/receiver_main.cpp)
add_executable(sender src/sender_main.cpp)

add_library(udprx_core STATIC 
    src/packet.cpp
    src/checksum.cpp 
)

target_include_directories(udprx_core PUBLIC include)

target_link_libraries(receiver PRIVATE udprx_core)
target_link_libraries(sender PRIVATE udprx_core)

