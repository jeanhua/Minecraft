cmake_minimum_required(VERSION 3.2)
project(Minecraft)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/package/include
)

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/package/lib
)

add_subdirectory(src)

add_executable(Minecraft
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/package/source/glad.c
)

target_link_libraries(
        Minecraft PRIVATE funcs
        glfw3
        FastNoise
)

file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/package/lib/FastNoise.dll DESTINATION ${CMAKE_BINARY_DIR}
)

file(
        COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR}
)