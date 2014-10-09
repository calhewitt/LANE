project(testPlugin)

set(TESTPLUGIN_SOURCES
    Main.cpp
    Sample.hpp
)

add_executable(testPlugin ${TESTPLUGIN_SOURCES})

target_link_libraries(testPlugin libLUCID)

set_target_properties(testPlugin
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)
