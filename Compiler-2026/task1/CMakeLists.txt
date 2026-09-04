cmake_minimum_required(VERSION 3.16)

project(SysYFrontendTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(BISON 3.2 REQUIRED)
find_package(FLEX 2.6 REQUIRED)

set(YACC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/yacc")
set(YACC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/yacc")
file(MAKE_DIRECTORY "${YACC_INCLUDE_DIR}")

BISON_TARGET(
    SysYParser
    "${YACC_SOURCE_DIR}/sysy.y"
    "${YACC_SOURCE_DIR}/Bison.cpp"
    DEFINES_FILE "${YACC_INCLUDE_DIR}/Bison.hpp"
)

FLEX_TARGET(
    SysYLexer
    "${YACC_SOURCE_DIR}/sysy.l"
    "${YACC_SOURCE_DIR}/Flex.cpp"
    DEFINES_FILE "${YACC_INCLUDE_DIR}/Flex.hpp"
)

ADD_FLEX_BISON_DEPENDENCY(SysYLexer SysYParser)

add_executable(
    sysy_frontend
    main.cpp
    src/lib/ASTPrinter.cpp
    ${BISON_SysYParser_OUTPUTS}
    ${FLEX_SysYLexer_OUTPUTS}
)

target_include_directories(
    sysy_frontend
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${YACC_INCLUDE_DIR}"
)

if(FLEX_LIBRARIES)
    target_link_libraries(sysy_frontend PRIVATE ${FLEX_LIBRARIES})
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sysy_frontend PRIVATE -Wall -Wextra -Wpedantic)
endif()
