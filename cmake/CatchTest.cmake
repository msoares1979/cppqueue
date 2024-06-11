# Catch2 testing support

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
