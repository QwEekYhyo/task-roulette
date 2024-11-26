cmake_minimum_required(VERSION 3.10)

project(task-roulette LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_SOURCES src/utils.c)
set(SERVER_SOURCES src/server_utils.c src/player.c)
include_directories(include)

add_executable(task-roulette-client src/client.c ${COMMON_SOURCES})
add_executable(task-roulette-server src/server.c ${COMMON_SOURCES} ${SERVER_SOURCES})

target_link_libraries(task-roulette-client PRIVATE paho-mqtt3c)
target_link_libraries(task-roulette-server PRIVATE paho-mqtt3c)
