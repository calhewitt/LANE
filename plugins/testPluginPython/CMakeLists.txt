project(testPluginPython)

set(PLUGIN_SOURCES
    testPluginPython.py
)

add_custom_target(${PROJECT_NAME})

foreach(source ${PLUGIN_SOURCES})
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E 
            copy ${CMAKE_CURRENT_SOURCE_DIR}/${source} ${CMAKE_BINARY_DIR}/bin/${source}
    )
endforeach()

add_dependencies(plugins ${PROJECT_NAME})
