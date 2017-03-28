cmake_minimum_required(VERSION 3.6)
project(raytracing)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")

set(SOURCE_FILES
        include/area_light.hpp
        include/camera.hpp
        include/color.hpp
        include/finite_planar_shape.hpp
        include/geometry_object.hpp
        include/light.hpp
        include/material.hpp
        include/normal_object.hpp
        include/object.hpp
        include/planar_shape.hpp
        include/plane_shape.hpp
        include/ray.hpp
        include/rectangle_shape.hpp
        include/renderer.hpp
        include/resources.hpp
        include/scene.hpp
        include/screen.hpp
        include/shape.hpp
        include/spot_light.hpp
        include/traceconfig.hpp
        include/tracer.hpp
        include/vec.hpp
        src/main.cpp)

add_executable(raytracing ${SOURCE_FILES})