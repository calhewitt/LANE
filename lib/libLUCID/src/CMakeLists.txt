project(libLUCID)

set(LIBLUCID_SOURCES
    Frame.cpp ../include/LUCID/Frame.hpp
    LUCIDFile.cpp ../include/LUCID/LUCIDFile.hpp
    Pixel.cpp ../include/LUCID/Pixel.hpp
)

add_library(libLUCID ${LIBLUCID_SOURCES})
