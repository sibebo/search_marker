cmake_minimum_required(VERSION 3.10)

# set the project name
project(search_marker)

include_directories(rang src)
# target_include_directories(search_marker src rang)

# add the executable
add_executable(search_marker src/main.cpp src/search_marker.h rang/rang.hpp)

