cmake_minimum_required(VERSION 3.5)

project(dlib)

add_executable(kkmeans main.cpp)

set_target_properties(kkmeans PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wall;-Wextra;-Wpedantic"
)

target_link_libraries(kkmeans
   dlib openblas
)