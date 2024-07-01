@echo off

:: Define build directory paths
set BUILD_DIR=build

:: Function to create build directory if it doesn't exist
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
    echo Created build directory: %BUILD_DIR%
)

:: Check if CMakeCache.txt exists in the build directory
if not exist %BUILD_DIR%\CMakeCache.txt (
    echo CMakeCache.txt not found. Running clean build.
    scripts\clean_build.bat
) 

:: Change to build directory
cd %BUILD_DIR%

:: Check if ALL_BUILD.vcxproj exists
if not exist ALL_BUILD.vcxproj (
    echo ALL_BUILD.vcxproj not found. Running clean build.
    cd ..
    scripts\clean_build.bat
    cd %BUILD_DIR%
)

:: Run MSBuild to build the project
msbuild ALL_BUILD.vcxproj /p:Configuration=Debug /t:Build

echo Incremental build completed.