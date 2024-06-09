cmake_minimum_required(VERSION 3.28)
project(asyncqueue LANGUAGES CXX)

include_directories(src)

# simple usage executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cc src/queue.h)

# cmake's test tool helper
include(CTest)

# test framework using Catch2 v2 single-header
enable_testing()
include_directories(test)

add_executable(test_basic test/basic.cc)
add_test(NAME test_basic
    COMMAND env LD_LIBRARY_PATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY} $<TARGET_FILE:test_basic>
)

add_custom_target(check COMMAND ctest --progress -V --output-on-failure)
