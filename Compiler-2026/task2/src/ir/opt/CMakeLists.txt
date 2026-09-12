add_library(Opt
    NewPassManager.cpp
    PassBuilder.cpp
    Mem2Reg.cpp
    LoopSimplify.cpp
    LCSSA.cpp
    LocalGVN.cpp
    LICM.cpp
)
target_link_libraries(Opt PUBLIC Lib Analysis)
target_include_directories(Opt PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
