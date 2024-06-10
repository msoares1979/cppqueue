cmake_minimum_required(VERSION 3.28)
project(asyncqueue LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fsanitize=undefined")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -fsanitize=address -fsanitize=undefined")

include_directories(src)

# simple usage executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cc src/queue.h)

# cmake's test tool helper
include(CTest)

# test framework using Catch2 v2 single-header
enable_testing()
include_directories(test)

add_executable(test_basic test/basic.cc src/queue.h)
add_test(
  NAME test_basic
  COMMAND $<TARGET_FILE:test_basic> -s -o ${CMAKE_CURRENT_BINARY_DIR}/test_basic.txt
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(test_threaded test/threaded.cc src/queue.h)
add_test(
  NAME test_threaded
  COMMAND $<TARGET_FILE:test_threaded> -s -o ${CMAKE_CURRENT_BINARY_DIR}/test_threaded.txt
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

get_property(
  TEST_NAMES
  DIRECTORY ${CMAKE_BINARY_DIR}
  PROPERTY TESTS
)

add_custom_target(
  check
  COMMAND ctest --progress --output-on-failure
  DEPENDS  ${TEST_NAMES}
)
