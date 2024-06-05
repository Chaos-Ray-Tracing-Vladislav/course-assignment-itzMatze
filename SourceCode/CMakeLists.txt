cmake_minimum_required(VERSION 3.19)
project(Chaos)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES src/color.cpp src/camera.cpp src/ray.cpp src/triangle.cpp
    src/random_generator.cpp src/image_factory.cpp src/stb_image_write.cpp)

add_executable(Chaos src/main.cpp ${SOURCE_FILES})

include_directories(Chaos PUBLIC "${PROJECT_SOURCE_DIR}/include")

