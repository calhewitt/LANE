##############################################################################
# rawToIntermediate conversion module build configuration script
project(rawToIntermediate)

set(plugin_sources
    src/rawToIntermediate.py
)

install(FILES ${plugin_sources} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/${PROJECT_NAME})
