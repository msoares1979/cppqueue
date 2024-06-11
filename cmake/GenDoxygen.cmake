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
  message(WARNING "Doxygen not found")
endif ()
