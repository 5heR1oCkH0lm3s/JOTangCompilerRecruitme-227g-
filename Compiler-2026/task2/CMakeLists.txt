cmake_minimum_required(VERSION 3.13)
project(compiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 默认构建带调试信息的中端，方便 VS Code / GDB 单步调试。
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "构建类型" FORCE)
endif()

message(STATUS "=== 构建中端调试版本 ===")

add_subdirectory(${PROJECT_SOURCE_DIR}/src/lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/yacc)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/ir)

# 前端生成 IR，中端优化后输出 .ll 文件。
add_executable(compiler_middle_end ${CMAKE_SOURCE_DIR}/main.cpp)
target_link_libraries(compiler_middle_end Lib Yacc Opt Analysis)
target_compile_options(compiler_middle_end PRIVATE -g -O0 -Wall -Wextra)
