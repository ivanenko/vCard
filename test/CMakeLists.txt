
find_package(Catch2 CONFIG REQUIRED)


add_executable(${CMAKE_PROJECT_NAME}_test main.cpp catch.hpp tests.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}_test ${CMAKE_PROJECT_NAME} Catch2::Catch2)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(${CMAKE_PROJECT_NAME}_test)
