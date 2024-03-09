set(CMAKE_CXX_STANDARD 20) 

cmake_minimum_required(VERSION 3.5)

# SOLVER

project (backpack)

add_executable(backpack input.cpp run_solver.cpp)

target_include_directories(backpack
    PRIVATE 
        ${PROJECT_SOURCE_DIR}
)


# CHECKER

project (checker)

add_executable(checker input.cpp checker.cpp)

target_include_directories(checker
    PRIVATE 
        ${PROJECT_SOURCE_DIR}
)