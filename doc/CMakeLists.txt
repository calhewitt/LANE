#=============================================================================
# Setup the documentation
#=============================================================================
if (NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found; the 'doc' target will be unavailable.")
    return()
endif()

configure_file(Doxyfile.in Doxyfile @ONLY)
add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    SOURCES Doxyfile
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)
