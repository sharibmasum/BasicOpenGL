cmake_minimum_required(VERSION 3.28)
project(BasicOpenGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/glad)
include_directories(/opt/homebrew/include)


link_directories(/opt/homebrew/lib)

set(SOURCES
        src/main.cpp
        src/Mesh.cpp
        src/Shader.cpp
        glad/gl.c
)

# Executable target
add_executable(BasicOpenGL ${SOURCES})

# Link GLFW and macOS system frameworks
target_link_libraries(BasicOpenGL
        glfw
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
)
