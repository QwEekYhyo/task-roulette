cmake_minimum_required(VERSION 3.10)

project(task-roulette LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task-roulette-client pub.c)
add_executable(task-roulette-server sub.c)

target_link_libraries(task-roulette-client PRIVATE paho-mqtt3c)
target_link_libraries(task-roulette-server PRIVATE paho-mqtt3c)
