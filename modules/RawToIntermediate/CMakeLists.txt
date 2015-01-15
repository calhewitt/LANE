##############################################################################
# RawToIntermediate module build configuration script
project(rawToIntermediate)



##############################################################################
# Build module
set(module_sources
    src/Main.cpp
)

add_executable(${PROJECT_NAME} ${module_sources})

target_link_libraries(${PROJECT_NAME} lane)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/${PROJECT_NAME})
