cmake_minimum_required(VERSION 3.8)
project(almo)

# setup

add_definitions(-DGLEW_STATIC)
set(CMAKE_CXX_STANDARD 17)

# packages

find_package(OpenGL REQUIRED)

# include paths

if (APPLE)
	include_directories(
        /usr/local/include
	)

	link_directories(
        /usr/local/lib
	)
endif(APPLE)

if (WIN32)
	include_directories(
        C:/Users/Albert/dev/libs/include/
	)

	link_directories(
        C:/Users/Albert/dev/libs/lib/
	)
endif(WIN32)

# sources files and the target

set(SOURCE_FILES
        almo/Display.cpp
        almo/Display.hpp
        almo/main.cpp
        almo/Program.cpp
        almo/Program.hpp
        almo/Shader.cpp
        almo/Shader.hpp
        almo/Camera.hpp
        almo/Camera.cpp
		almo/Engine.hpp
		almo/Engine.cpp
)

add_executable(almo ${SOURCE_FILES})

# link libraries

if (APPLE)
	target_link_libraries(almo
        SDL2
        GLEW
        ${OPENGL_LIBRARIES})
endif(APPLE)

if (WIN32)
	target_link_libraries(almo
        SDL2.lib
		SDL2main.lib
        glew32s.lib
        ${OPENGL_LIBRARIES})

	file(COPY C:/Users/Albert/dev/libs/lib/SDL2.dll
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif (WIN32)

# Copy shader files

file(COPY almo/simple.frag
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY almo/simple.vert
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})