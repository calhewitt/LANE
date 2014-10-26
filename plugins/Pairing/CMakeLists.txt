project(Pairing)

set(PLUGIN_SOURCES
    src/Pairing.py
    src/ReadBCA.py
)

add_custom_target(${PROJECT_NAME})

function(add_plugin_target src)
    get_filename_component(filename ${src} NAME)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E 
            make_directory ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME} &&
            ${CMAKE_COMMAND} -E 
            copy ${CMAKE_CURRENT_SOURCE_DIR}/${src} ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}/${filename}
    )
endfunction()

foreach(source ${PLUGIN_SOURCES})
    add_plugin_target(${source})
endforeach()

add_dependencies(plugins ${PROJECT_NAME})
