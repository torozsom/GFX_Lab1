cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(Lab1)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set paths for Glad
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/glad/include)
set(GLAD_SRC ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)

# Add include directories
include_directories(${GLAD_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/sources)

# GLFW - If installed globally, find it
find_package(glfw3 REQUIRED)

# Source files
set(SOURCES
        sources/framework.cpp
        sources/lodepng.cpp
        ${GLAD_SRC}  # Add GLAD source file
)

# Header files (for clarity, optional)
set(HEADERS
        sources/framework.h
        sources/lodepng.h
)

# Create executable
add_executable(Lab1 ${SOURCES} ${HEADERS}
        sources/MyApp.cpp
        sources/PointCollection.cpp
        sources/PointCollection.h
        sources/Line.cpp
        sources/Line.h
        sources/LineCollection.cpp
        sources/LineCollection.h
)

# Link libraries
target_link_libraries(Lab1 OpenGL::GL glfw)
