#=============================================================================
# Setup project
#=============================================================================
cmake_minimum_required(VERSION 2.8.8)
project(RawToIntermediate)


#=============================================================================
# Setup CMake options
#=============================================================================
include(CMakeDependentOption)
option(ENABLE_CXX11 "Compile with C++11 enabled." ON)
option(ENABLE_PEDANTIC "Compile with pedantic enabled." ON)
option(ENABLE_WERROR "Fail and stop if a warning is triggered." OFF)
option(ENABLE_EXCEPTIONS "Enable exceptions" ON)
option(ENABLE_WALL "Set warning level to the highest level." ON)
option(ENABLE_WEXTRA "Check for additional warnings" ON)
option(ENABLE_WNO_UNUSED_LOCAL_TYPEDEFS "Warn on unused local typedefs" ON)
option(ENABLE_WNO_UNUSED_PARAMETER "Warn on unused parameters" ON)
option(ENABLE_WNO_LONG_LONG "Warn if long long type is used." ON)
option(ENABLE_FTEMPLATE_BACKTRACE_LIMIT_0 "Set the maximum number of template instantiation notes for a single warning or error to 0." ON)


#=============================================================================
# Check available compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-pedantic HAS_PEDANTIC_FLAG)
check_cxx_compiler_flag(-std=c++11 HAS_STDCXX11_FLAG)
check_cxx_compiler_flag(-fexceptions HAS_EXCEPTIONS_FLAG)
check_cxx_compiler_flag(-fno-exceptions HAS_NOEXCEPTIONS_FLAG)
check_cxx_compiler_flag(-W HAS_W_FLAG)
check_cxx_compiler_flag(-Wall HAS_WALL_FLAG)
check_cxx_compiler_flag(-Werror HAS_WERROR_FLAG)
check_cxx_compiler_flag(-Wextra HAS_WEXTRA_FLAG)
check_cxx_compiler_flag(-Wno-long-long HAS_WNO_LONG_LONG_FLAG)
check_cxx_compiler_flag(-Wno-unused-local-typedefs HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG)
check_cxx_compiler_flag(-Wno-unused-parameter HAS_WNO_UNUSED_PARAMETER_FLAG)
check_cxx_compiler_flag(-ftemplate-backtrace-limit=0 HAS_FTEMPLATE_BACKTRACE_LIMIT_0_FLAG)


#=============================================================================
# Setup compiler/linker flags
#=============================================================================
macro(append_if condition lst var)
    if (${condition})
        list(APPEND ${lst} ${var})
    endif()
endmacro()

if (ENABLE_WALL)
    append_if(HAS_WALL_FLAG CXX_WARNING_FLAGS -Wall)
endif()

if (ENABLE_WEXTRA)
    append_if(HAS_WEXTRA_FLAG CXX_WARNING_FLAGS -Wextra)
endif()

if (ENABLE_WNO_UNUSED_LOCAL_TYPEDEFS)
    append_if(HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG CXX_WARNING_FLAGS -Wno-unused-local-typedefs)
endif()

if (ENABLE_WNO_UNUSED_PARAMETER)
    append_if(HAS_WNO_UNUSED_PARAMETER_FLAG CXX_WARNING_FLAGS -Wno-unused-parameter)
endif()

if (ENABLE_WNO_LONG_LONG_FLAG)
    append_if(HAS_WNO_LONG_LONG_FLAG CXX_WARNING_FLAGS -Wno-long-long)
endif()

if (ENABLE_FTEMPLATE_BACKTRACE_LIMIT_0)
    append_if(HAS_FTEMPLATE_BACKTRACE_LIMIT_0_FLAG CXX_WARNING_FLAGS -ftemplate-backtrace-limit=0)
endif()

if (ENABLE_WERROR)
    append_if(HAS_WERROR_FLAG CXX_WARNING_FLAGS -Werror)
    append_if(HAS_WX_FLAG CXX_WARNING_FLAGS -WX)
endif()

if (ENABLE_PEDANTIC)
    append_if(HAS_PEDANTIC_FLAG CXX_WARNING_FLAGS -pedantic)
endif()

if (ENABLE_EXCEPTIONS)
    append_if(HAS_EXCEPTIONS_FLAG CXX_WARNING_FLAGS -fexceptions)
else ()
    append_if(HAS_NOEXCEPTIONS_FLAG CXX_WARNING_FLAGS -fno-exceptions)
endif()

if (ENABLE_CXX11)
    append_if(HAS_STDCXX11_FLAG CXX_FEATURE_FLAGS -std=c++11)
endif()


# This is the only place where add_definitions is called. Other properties
# are set on a per-target basis.
add_definitions(
    ${CXX_WARNING_FLAGS}
    ${CXX_FEATURE_FLAGS}
)


#=============================================================================
# Build plugin
#=============================================================================
set(PLUGIN_SOURCES
    src/Main.cpp
)

add_executable(${PROJECT_NAME} ${PLUGIN_SOURCES})

target_link_libraries(${PROJECT_NAME} libLANE)

set_target_properties(${PROJECT_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin
)

add_dependencies(plugins ${PROJECT_NAME})