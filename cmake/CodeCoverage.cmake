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
else ()
  message(WARNING "lcov or genhtml not found")
endif()
