project(SIGNAL)
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
# FILE(GLOB SRC_FILES main.cpp)
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE Signals)