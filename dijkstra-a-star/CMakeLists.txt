cmake_minimum_required(VERSION 3.13)  # CMake version check
project(simple_example)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 20)            # Enable c++14 standard

# add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-Wall)

option(SANITIZE "Sanitizers" OFF)
IF (SANITIZE)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address -g)
endif(SANITIZE)

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES main.cpp src/code/estimator.cpp src/code/progress_bar.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(a.out ${SOURCE_FILES})
