cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(engine 
    src/main.cpp 
    src/board.cpp 
    src/movegen.cpp 
    src/bitutils.cpp
    src/evaluate.cpp
    src/search.cpp
)