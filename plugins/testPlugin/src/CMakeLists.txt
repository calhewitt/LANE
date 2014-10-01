project(TestPlugin)

set(TESTPLUGIN_SOURCES
    Main.cpp
    Sample.hpp
)

add_executable(TestPlugin ${TESTPLUGIN_SOURCES})

set_target_properties(TestPlugin
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)
