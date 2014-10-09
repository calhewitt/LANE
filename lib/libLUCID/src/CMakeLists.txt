project(libLUCID)

if (WIN32)
    set(FILESYSTEM_SOURCES
         Utils/FilesystemWindows.cpp
    )
elseif(UNIX OR APPLE)
    set(FILESYSTEM_SOURCES
         Utils/FilesystemLinux.cpp
    )
else()
    message(FATAL_ERROR "libLUCID doesn't support this platform")
endif(WIN32)

set(LIBLUCID_SOURCES
    Frame.cpp ../include/Frame.hpp
    LUCID/LUCIDFile.cpp ../include/LUCID/LUCIDFile.hpp
    Pixel.cpp ../include/Pixel.hpp
    Utils/Filesystem.cpp ${FILESYSTEM_SOURCES} ../include/Utils/Filesystem.hpp 
)

add_library(libLUCID ${LIBLUCID_SOURCES})
