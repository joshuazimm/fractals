cmake_minimum_required(VERSION 3.10)
project(Fractal)

# Set the output directories for different build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set(CMAKE_SUPPRESS_REGENERATION true)

# Avoid creating unnecessary intermediate directories
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Find all .c and .h files in src directory
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE HDR_FILES "${CMAKE_SOURCE_DIR}/src/*.h")

# Add executable
add_executable(${PROJECT_NAME} ${SRC_FILES} ${HDR_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/dep/glfw/include
    ${CMAKE_SOURCE_DIR}/dep/glew/include
    ${CMAKE_SOURCE_DIR}/dep/cglm/include
)

# Set GLEW to build as a static library
set(BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
set(BUILD_UTILS OFF CACHE BOOL " " FORCE)

# Dependencies
add_subdirectory(dep/glfw)
add_subdirectory(dep/glew/build/cmake)
add_subdirectory(dep/cglm)

set_property(TARGET cglm PROPERTY FOLDER "Libs")

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    libglew_static  # Adjust this if the library name is different
    cglm
)

# Define GLEW_STATIC
target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

# Additional system libraries on Windows
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
endif()