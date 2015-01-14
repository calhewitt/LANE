###############################################################################
# Lane lib build configuration script
project(lane)
if (CMAKE_COMPILER_IS_GNUCXX)
    #set(CXX_WARNINGS "-Wall -Wextra -Wpointer-arith -Wcast-align -fstrict-aliasing -Wno-unused-local-typedefs")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS} -fvisibility-inlines-hidden")
endif()

include_directories(
    include
)

if (WIN32)
    set(filesystem_sources
         src/Utils/FilesystemWindows.cpp
    )
elseif(UNIX OR APPLE)
    set(filesystem_sources
         src/Utils/FilesystemLinux.cpp
    )
else()
    message(FATAL_ERROR "lane library doesn't support this platform")
endif(WIN32)

set(lanelib_includes
    include/Frame.hpp
    include/RawInputFile.hpp
    include/LaneFile.hpp
    include/Lucid/LucidFile.hpp
    include/Pixel.hpp
    include/Blob.hpp
    include/BlobFinder.hpp
    include/Utils/Logger.hpp
    include/Utils/LoggerSink.hpp
    include/Utils/Filesystem.hpp
)

set(lanelib_sources
    src/Frame.cpp
    src/LaneFile.cpp 
    src/Lucid/LucidFile.cpp 
    src/Pixel.cpp 
    src/Blob.cpp 
    src/BlobFinder.cpp 
    src/Utils/Logger.cpp 
    src/Utils/LoggerSink.cpp 
    src/Utils/Filesystem.cpp ${filesystem_sources} 
)

add_library(lane STATIC ${lanelib_sources} ${lanelib_includes})
