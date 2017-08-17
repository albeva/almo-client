cmake_minimum_required(VERSION 3.8)
project(almo)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)

include_directories(
        /usr/local/include
)

link_directories(
        /usr/local/lib
)

set(SOURCE_FILES
        almo/Display.cpp
        almo/Display.hpp
        almo/main.cpp
        almo/Program.cpp
        almo/Program.hpp
        almo/Shader.cpp
        almo/Shader.hpp)

add_executable(almo ${SOURCE_FILES})

target_link_libraries(almo
        SDL2
        GLEW
        ${OPENGL_LIBRARIES})

file(COPY almo/simple.frag
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY almo/simple.vert
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})