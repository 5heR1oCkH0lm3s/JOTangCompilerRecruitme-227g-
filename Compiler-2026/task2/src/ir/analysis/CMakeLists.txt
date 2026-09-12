add_library(Analysis
    Dominant.cpp
    LoopInfo.cpp
)
target_link_libraries(Analysis PUBLIC Lib)
target_include_directories(Analysis PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
