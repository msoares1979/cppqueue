cmake_minimum_required(VERSION 3.28)
project(asyncqueue LANGUAGES CXX VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CXX_COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fsanitize=undefined ${CXX_COVERAGE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -fsanitize=address -fsanitize=undefined ${CXX_COVERAGE_FLAGS}")

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

# include doxygen support
option(BUILD_DOXYGEN "Build doxygen documentation" ON)

# Check if Doxygen is installed
find_package(Doxygen)
if (BUILD_DOXYGEN AND DOXYGEN_FOUND)

    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      @ONLY
    )

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
else ()
    message(STATUS "Doxygen not found")
endif ()

# generate code coverage report
find_program(LCOV lcov)
find_program(GENHTML genhtml)
if (LCOV AND GENHTML)
  message(STATUS "Found lcov and genhtml")
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/coverage)

  add_custom_target(coverage ALL
      COMMAND ${LCOV} --directory ${CMAKE_BINARY_DIR} --zerocounters
      COMMAND env -C ${CMAKE_BINARY_DIR} ctest -j ${PROCESSOR_COUNT}
      COMMAND ${LCOV} --directory ${CMAKE_BINARY_DIR} --capture --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info
      COMMAND ${LCOV} --ignore-errors unused
          --remove ${CMAKE_BINARY_DIR}/coverage/coverage.info /usr/*
          --remove ${CMAKE_BINARY_DIR}/coverage/coverage.info /nix/*
          --remove ${CMAKE_BINARY_DIR}/coverage/coverage.info ${CMAKE_SOURCE_DIR}/test/*
          --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info
      COMMAND ${LCOV} --list ${CMAKE_BINARY_DIR}/coverage/coverage.info
      COMMAND ${GENHTML} ${CMAKE_BINARY_DIR}/coverage/coverage.info --output-directory ${CMAKE_BINARY_DIR}/coverage/html
      COMMAND ${LCOV} --summary ${CMAKE_BINARY_DIR}/coverage/coverage.info --fail-under-lines 80
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/coverage
      VERBATIM
  )
endif()
