find_package(Doxygen)

include_directories(lib/libLANE/include)

add_custom_target(plugins ALL)
add_custom_target(tools ALL)

add_subdirectory(doc)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(plugins)
