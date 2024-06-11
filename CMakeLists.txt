cmake_minimum_required(VERSION 3.28)
project(asyncqueue LANGUAGES CXX VERSION 1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
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

include(CatchTest)

include(GenDoxygen)

include(CodeCoverage)

include(FeatureSummary)
feature_summary(WHAT ALL DESCRIPTION "Project ${PROJECT_NAME} features:")
