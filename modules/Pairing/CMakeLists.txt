##############################################################################
# pairing module build configuration script
project(pairing)

set(plugin_sources
    src/Pairing.py
    src/ReadBCA.py
)

install(FILES ${plugin_sources} DESTINATION ${CMAKE_INSTALL_PREFIX}/modules/${PROJECT_NAME})
