cmake_minimum_required(VERSION 2.8.8)
project(libLANEpy)

set(LIBLANEPY_SOURCES
    src/ReadBCA.py
)

add_custom_target(${PROJECT_NAME})

function(add_plugin_target src)
    get_filename_component(filename ${src} NAME)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E 
            copy ${CMAKE_CURRENT_SOURCE_DIR}/${src} ${CMAKE_BINARY_DIR}/bin/${filename}
    )
endfunction()

foreach(source ${LIBLANEPY_SOURCES})
    add_plugin_target(${source})
endforeach()

add_dependencies(plugins ${PROJECT_NAME})
