cmake_minimum_required(VERSION 3.10)

# Define constants for project name and target name
set(PROJECT_NAME FractalPrj)
set(TARGET_NAME Fractal)

project(${PROJECT_NAME})

# Set the output directories for different build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add executable target
add_executable(${TARGET_NAME} src/main.c)

# Add the minifb submodule as an external library
add_subdirectory(dep/minifb)
add_subdirectory(dep/cglm)

# Include submod headers and link libraries to executable
target_include_directories(${TARGET_NAME} PRIVATE dep/minifb/include)
target_include_directories(${TARGET_NAME} PRIVATE dep/cglm/include)
target_link_libraries(${TARGET_NAME} PRIVATE minifb)
target_link_libraries(${TARGET_NAME} PRIVATE cglm)