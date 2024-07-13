cmake_minimum_required(VERSION 3.28)
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
    ${CMAKE_SOURCE_DIR}/dep/glm/include
)

# Dependencies
add_subdirectory(dep/glfw)
add_subdirectory(dep/glew)
add_subdirectory(dep/glm)

set_property(TARGET libglew_shared PROPERTY FOLDER "Libs")
set_property(TARGET libglew_static PROPERTY FOLDER "Libs")
set_property(TARGET glfw PROPERTY FOLDER "Libs")
set_property(TARGET uninstall PROPERTY FOLDER "Libs")
set_property(TARGET update_mappings PROPERTY FOLDER "Libs")

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    libglew_static
    glm
)