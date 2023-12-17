
cmake_minimum_required(VERSION 3.0)  # CMake version check
project(GeoMod)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES main.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(GeoMod ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
target_link_libraries(GeoMod PUBLIC OpenGL::GL OpenGL::GLU)