cmake_minimum_required(VERSION 3.16)

project(SysYFrontendTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(BISON 3.2 REQUIRED)
find_package(FLEX 2.6 REQUIRED)

set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${GENERATED_DIR}")

BISON_TARGET(
    SysYParser
    "${CMAKE_CURRENT_SOURCE_DIR}/src/yacc/sysy.y"
    "${GENERATED_DIR}/Bison.cpp"
    DEFINES_FILE "${GENERATED_DIR}/Bison.hpp"
)

FLEX_TARGET(
    SysYLexer
    "${CMAKE_CURRENT_SOURCE_DIR}/src/yacc/sysy.l"
    "${GENERATED_DIR}/Flex.cpp"
    DEFINES_FILE "${GENERATED_DIR}/Flex.hpp"
)

ADD_FLEX_BISON_DEPENDENCY(SysYLexer SysYParser)

add_executable(
    sysy_frontend
    main.cpp
    ${BISON_SysYParser_OUTPUTS}
    ${FLEX_SysYLexer_OUTPUTS}
)

target_include_directories(
    sysy_frontend
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${GENERATED_DIR}"
)

if(FLEX_LIBRARIES)
    target_link_libraries(sysy_frontend PRIVATE ${FLEX_LIBRARIES})
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sysy_frontend PRIVATE -Wall -Wextra -Wpedantic)
endif()
