cmake_minimum_required(VERSION 3.13)
project(vCard VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_subdirectory(src)
ENABLE_TESTING()
add_subdirectory(test)
