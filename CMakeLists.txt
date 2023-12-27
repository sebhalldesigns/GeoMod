
cmake_minimum_required(VERSION 3.0)  # CMake version check
project(GeoMod)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 17)            # Enable c++14 standard

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES main.cpp)

file(GLOB IMGUI_SRC
     "imgui/*.cpp"
)


# Add executable target with source files listed in SOURCE_FILES variable
add_executable(GeoMod ${SOURCE_FILES} ${IMGUI_SRC})
find_package(OpenGL REQUIRED)

target_include_directories(GeoMod PUBLIC "SDL/")
target_link_libraries(GeoMod PUBLIC OpenGL::GL)
target_link_libraries(GeoMod PUBLIC ${CMAKE_SOURCE_DIR}/lib/SDL2.lib ${CMAKE_SOURCE_DIR}/lib/SDL2main.lib ${CMAKE_SOURCE_DIR}/lib/glew32.lib)