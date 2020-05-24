cmake_minimum_required(VERSION 3.10)

project(VisualProfiler)

set(SOURCES
    VisualProfiler.hpp
    main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
