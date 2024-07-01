@echo off

:: Define build directory path
set BUILD_DIR=build

:: Check if build directory exists
if exist %BUILD_DIR% (
    echo Cleaning build directory...
    rmdir /s /q %BUILD_DIR%
    mkdir %BUILD_DIR%
    echo Build directory cleaned.
) else (
    echo No build directory found.
)