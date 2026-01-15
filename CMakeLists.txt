cmake_minimum_required( VERSION 3.14 )
project( TinyJson LANGUAGES CXX VERSION 1.0.0 )

# ==============================================================================
# [Global Project Settings]
# ==============================================================================
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF ) # Ensure standard compliance

# ==============================================================================
# [Dependency Management: CPM]
# ==============================================================================
set( CPM_DOWNLOAD_VERSION 0.38.2 )
set( CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake" )

if( NOT ( EXISTS ${CPM_DOWNLOAD_LOCATION} ) )
    message( STATUS "Downloading CPM.cmake..." )
    file( DOWNLOAD
          https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
          ${CPM_DOWNLOAD_LOCATION}
    )
endif()

include( ${CPM_DOWNLOAD_LOCATION} )

# ==============================================================================
# [Library Target: TinyJson]
# ==============================================================================
add_library( tinyjson_lib STATIC
    src/TinyJson.cpp
    include/TinyJson.h
)

# Allow usage like #include "TinyJson.h"
target_include_directories( tinyjson_lib PUBLIC include )

# Compiler Warnings
if( MSVC )
    target_compile_options( tinyjson_lib PRIVATE /W4 )
else()
    target_compile_options( tinyjson_lib PRIVATE -Wall -Wextra )
endif()

# ==============================================================================
# [Example Executable]
# ==============================================================================
add_executable( TinyJsonExample main.cpp )

target_link_libraries( TinyJsonExample PRIVATE tinyjson_lib )

# Output binary to project root for easy execution
set_target_properties( TinyJsonExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# ==============================================================================
# [Unit Tests with Catch2]
# ==============================================================================
CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    VERSION 3.5.0
)

if( Catch2_ADDED )
    enable_testing()

    add_executable( unit_tests tests/test_main.cpp )

    # Output binary to project root
    set_target_properties( unit_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )

    target_link_libraries( unit_tests PRIVATE tinyjson_lib Catch2::Catch2WithMain )

    # CTest Integration
    include( CTest )
    list( APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras" )
    include( Catch )
    catch_discover_tests( unit_tests )

    message( STATUS ">> Unit Test Target Enabled: 'unit_tests'" )
endif()