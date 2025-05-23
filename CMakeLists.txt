﻿cmake_minimum_required(VERSION 3.8)
project(Particle)

set(CMAKE_CXX_STANDARD        20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG     "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od /RTC1")

# Пути к заголовкам (публичные API + GLFW + GLM)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/glfw/include
    ${PROJECT_SOURCE_DIR}/external/glm
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/
)

# GLFW и OpenGL
add_subdirectory(external/glfw)
find_package(OpenGL REQUIRED)

# Собираем все .cpp и пример
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/examples/minimal/main.cpp
    ${PROJECT_SOURCE_DIR}/external/glad/src/glad.c
)

# Единый исполняемый
add_executable(Particle ${SRC_FILES} "include/particle/shape/IEmitterShape.hpp" "src/shape/CircleShape.cpp" "src/shape/BoxShape.cpp"  "src/particles/SpriteRenderer.cpp")
target_link_libraries(Particle PRIVATE
    glfw
    OpenGL::GL
)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/textures
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
)
