
find_package(Catch2 CONFIG REQUIRED)


add_executable(${PROJECT_NAME}_test main.cpp catch.hpp tests.cpp)

target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} Catch2::Catch2)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(${PROJECT_NAME}_test)
