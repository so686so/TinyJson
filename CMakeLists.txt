cmake_minimum_required( VERSION 3.8 )

project( TinyJson )

# specify the C++ standard
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -fPIC" )

option( UNIT_TEST "Use unit_test for tinyjson" OFF )

if(UNIT_TEST)
    # gcov 및 lcov를 이용하여 코드 커버리지를 측정하기 위한 설정 추가
    set(CMAKE_C_OUTPUT_EXTENSION_REPLACE ON)
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
    add_compile_options(-g -O0 -fprofile-arcs -ftest-coverage --coverage)
    add_link_options(--coverage)
    link_libraries(gcov)
endif()

include_directories( ${PROJECT_SOURCE_DIR} )

# TinyJson Library ==========================================
set( LIBRARY_NAME tinyjson )
add_library( ${LIBRARY_NAME}
SHARED
    src/TinyJson.cpp
)
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER( ${LIBRARY_NAME} )
# TinyJson Library ==========================================

# TinyJson Example Program ==================================
set( EXECUTABLE_NAME TinyJsonExample )
add_executable( ${EXECUTABLE_NAME}
    src/main.cpp
)
target_include_directories( ${EXECUTABLE_NAME}
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries( ${EXECUTABLE_NAME}
    ${LIBRARY_NAME}
)
# TinyJson Example Program ==================================

IF (UNIT_TEST)
    set( TEST_TARGET_NAME TinyJsonUnitTest )
    add_executable( ${TEST_TARGET_NAME}
        test/TinyJsonUnitTest.cc
    )
    target_include_directories( ${TEST_TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        /usr/local/include
    )
    target_link_directories( ${TEST_TARGET_NAME}
    PRIVATE
        /usr/local/lib
    )
    target_link_libraries( ${TEST_TARGET_NAME}
    PRIVATE
        ${LIBRARY_NAME}
        gtest
        gtest_main
        pthread
    )

    # 단위테스트 수행 및 코드 커버리지 측정 레포트 생성
    add_custom_command( TARGET ${TEST_TARGET_NAME} POST_BUILD
                        COMMENT "Execute all unit test cases and generate coverage report"
                        COMMAND ./${TEST_TARGET_NAME}
                        COMMAND lcov --directory . --capture --output-file app.info
                        COMMAND genhtml -o ${CMAKE_CURRENT_SOURCE_DIR}/cov_html app.info )
endif()