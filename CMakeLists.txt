cmake_minimum_required( VERSION 3.14 )
project( TinyJson LANGUAGES CXX )

# C++ 표준 설정
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# ==============================================================================
# [CPM Package Manager 설정]
# ==============================================================================
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.2/CPM.cmake
  ${CMAKE_BINARY_DIR}/CPM.cmake
)
include( ${CMAKE_BINARY_DIR}/CPM.cmake )

# ==============================================================================
# [TinyJson 라이브러리 타겟 설정]
# ==============================================================================
add_library( tinyjson_lib STATIC
    src/TinyJson.cpp
    include/TinyJson.h
)

target_include_directories( tinyjson_lib PUBLIC include )

if( MSVC )
    target_compile_options( tinyjson_lib PRIVATE /W4 )
else()
    target_compile_options( tinyjson_lib PRIVATE -Wall -Wextra )
endif()

# ==============================================================================
# [Example 실행 파일 설정]
# ==============================================================================
add_executable( TinyJsonExample main.cpp )

target_link_libraries( TinyJsonExample PRIVATE tinyjson_lib )

# [추가됨] 실행 파일 출력 경로를 프로젝트 루트(${CMAKE_SOURCE_DIR})로 변경
set_target_properties( TinyJsonExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# ==============================================================================
# [Catch2 단위 테스트 설정]
# ==============================================================================
CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    VERSION 3.5.0
)

if( Catch2_ADDED )
    add_executable( unit_tests tests/test_main.cpp )

    target_link_libraries( unit_tests PRIVATE tinyjson_lib Catch2::Catch2WithMain )

    include( CTest )
    list( APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras" )
    include( Catch )
    catch_discover_tests( unit_tests )

    message( STATUS "Unit tests target 'unit_tests' created." )
endif()