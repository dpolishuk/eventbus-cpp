project(eventbus-cpp)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

add_definitions(-std=gnu++11)
add_executable(${PROJECT_NAME} ${SRC_LIST})
