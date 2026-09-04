cmake_minimum_required(VERSION 3.16)

project(SysYFrontendTask LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(YACC_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/yacc")
set(YACC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/yacc")

set(GENERATED_FILES
    "${YACC_SOURCE_DIR}/Bison.cpp"
    "${YACC_SOURCE_DIR}/Flex.cpp"
    "${YACC_INCLUDE_DIR}/Bison.hpp"
    "${YACC_INCLUDE_DIR}/Flex.hpp"
)

foreach(GENERATED_FILE IN LISTS GENERATED_FILES)
    if(NOT EXISTS "${GENERATED_FILE}")
        message(
            FATAL_ERROR
            "缺少生成文件：${GENERATED_FILE}\n"
            "请先执行 README.md 中的 Bison 和 Flex 命令，再配置 CMake。"
        )
    endif()
endforeach()

add_executable(
    sysy_frontend
    main.cpp
    src/lib/ASTPrinter.cpp
    src/yacc/Bison.cpp
    src/yacc/Flex.cpp
)

target_include_directories(
    sysy_frontend
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${YACC_INCLUDE_DIR}"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sysy_frontend PRIVATE -Wall -Wextra -Wpedantic)
endif()
