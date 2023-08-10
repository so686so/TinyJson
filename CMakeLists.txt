cmake_minimum_required( VERSION 3.8 )

project( TinyJson )

# specify the C++ standard
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -fPIC" )

option( UVES_UNIT_TEST "Use unit_test for tinyjson" OFF )

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

set( LIBRARY_NAME tinyjson )
set( LIBRARY_SRC_LIST
    src/TinyJson.cpp
)

add_library( ${LIBRARY_NAME}
SHARED
    ${LIBRARY_SRC_LIST}
)

include(GenerateExportHeader)
GENERATE_EXPORT_HEADER( ${LIBRARY_NAME} )

set( EXECUTABLE_NAME TinyJsonExample )
add_executable( ${EXECUTABLE_NAME}
    src/main.cpp
)
target_include_directories( ${EXECUTABLE_NAME}
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries( ${EXECUTABLE_NAME}
    tinyjson
)