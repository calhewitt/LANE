find_package(Doxygen)

include_directories(lib/libLUCID/include)

add_subdirectory(doc)
add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(plugins)
