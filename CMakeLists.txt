cmake_minimum_required(VERSION 3.10)

# Define constants for project name and target name
set(PROJECT_NAME FractalPrj)
set(TARGET_NAME Fractal)

project(${PROJECT_NAME})

# Set the output directories for different build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add the minifb submodule as an external library
add_subdirectory(dep/minifb)

# Add your main executable target
add_executable(${TARGET_NAME} src/main.c)

# Include the minifb headers
target_include_directories(${TARGET_NAME} PRIVATE dep/minifb/include)

# Link minifb library to your executable
target_link_libraries(${TARGET_NAME} PRIVATE minifb)