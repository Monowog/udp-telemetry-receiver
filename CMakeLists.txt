cmake_minimum_required(VERSION 3.23)

project(MyProjectName)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(receiver src/receiver_main.cpp)
add_executable(sender src/sender_main.cpp)

 target_include_directories(receiver PRIVATE src)
 target_include_directories(sender PRIVATE src)
