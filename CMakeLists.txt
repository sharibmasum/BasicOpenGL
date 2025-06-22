cmake_minimum_required(VERSION 3.28)
project(BasicOpenGL)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/glad)
include_directories(${CMAKE_SOURCE_DIR}/Shaders)
include_directories(${CMAKE_SOURCE_DIR}/stb)
include_directories(/opt/homebrew/include)


link_directories(/opt/homebrew/lib)

set(SOURCES
        src/main.cpp
        src/Mesh.cpp
        src/Shader.cpp
        glad/gl.c
        src/Window.cpp
        include/Window.h
        src/Camera.cpp
        include/Camera.h
        src/Texture.cpp
        include/Texture.h
        src/Light.cpp
        include/Light.h
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
