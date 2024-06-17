cmake_minimum_required(VERSION 3.19)
project(Chaos)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES src/camera.cpp src/ray.cpp src/triangle.cpp src/geometry.cpp src/scene.cpp src/renderer.cpp src/color.cpp
    src/random_generator.cpp src/image_factory.cpp src/scene_factory.cpp src/scene_builder.cpp
    src/file_handler.cpp src/stb_image_write.cpp)

add_executable(Chaos src/main.cpp ${SOURCE_FILES})
target_include_directories(Chaos PRIVATE "${PROJECT_SOURCE_DIR}/include")

add_executable(hw04 src/hw04.cpp)
target_include_directories(hw04 PRIVATE "${PROJECT_SOURCE_DIR}/include")

