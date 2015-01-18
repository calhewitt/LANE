###############################################################################
# Lane build configuration script
cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})



###############################################################################
# Compiler checks
include(CXX11)
check_for_cxx11_compiler(CXX11_COMPILER)

if(CXX11_COMPILER)
    enable_cxx11()
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} seems to have no \
C++11 support. Please try again with a more recent compiler version.")
endif()


###############################################################################
# Setup base install commands
install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/modules)")
install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/data)")
install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/masks)")
install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/calibrations)")
install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/configurations)")
install(DIRECTORY ${CMAKE_SOURCE_DIR}/scripts DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${CMAKE_SOURCE_DIR}/configLane.py DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${CMAKE_SOURCE_DIR}/runLane.py DESTINATION ${CMAKE_INSTALL_PREFIX})



###############################################################################
# Configure lib
# Configure lane lib
add_subdirectory(lib/lane)



###############################################################################
# Configure modules
include_directories(lib/lane/include)
add_subdirectory(modules)
