cmake_minimum_required(VERSION 3.19)
project(Chaos)
set(CMAKE_CXX_STANDARD 20)

add_executable(Chaos src/main.cpp ${SOURCE_FILES})

include_directories(Chaos PUBLIC "${PROJECT_SOURCE_DIR}/include")

