##############################################################################
# basicClusterAnalysis module build configuration script
project(BasicClusterAnalysis)



##############################################################################
# Build module
set(module_sources
    src/BasicClusterAnalysis.cpp
    src/BasicClusterAnalysis.hpp
    src/Main.cpp
)

add_executable(${PROJECT_NAME} ${module_sources})

target_link_libraries(${PROJECT_NAME} lane)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/${PROJECT_NAME})
