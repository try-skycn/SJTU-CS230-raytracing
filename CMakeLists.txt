cmake_minimum_required(VERSION 3.6)
project(raytracing)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

set(SOURCE_FILES
        include/color.hpp
        include/material.hpp
        include/object.hpp
        include/ray.hpp
        include/scene.hpp
        include/screen.hpp
        include/tracer.hpp
        include/vec.hpp
        src/main.cpp)

add_executable(raytracing ${SOURCE_FILES})