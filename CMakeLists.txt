cmake_minimum_required(VERSION 3.13)
project(vCard VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_executable(tests src/vcard.h src/vcard.cpp test/main.cpp test/catch.hpp test/tests.cpp src/text_io.h src/text_io.cpp src/xml_io.h src/xml_io.cpp src/utils.cpp src/utils.h src/json_io.h src/json_io.cpp)
include_directories(include)

