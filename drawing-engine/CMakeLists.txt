cmake_minimum_required(VERSION 3.16)

project(DrawingEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SDL3 CONFIG REQUIRED)

add_executable(drawing_engine
    src/main.cpp
    src/Canvas.cpp
    src/ImageWriter.cpp
    src/Drawing.cpp
)

target_include_directories(drawing_engine PRIVATE include)
target_link_libraries(drawing_engine PRIVATE SDL3::SDL3)