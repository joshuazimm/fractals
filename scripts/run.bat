@echo off

:: Save current directory and change to solution directory
set CURRENT_DIR=%CD%
cd /d %~dp0

:: Define executable path
set EXE_DIR=%~dp0build\bin\Debug
set EXE_NAME=project_name.exe

:: Check if executable exists
if not exist %EXE_DIR%\%EXE_NAME% (
    echo Executable %EXE_NAME% not found in %EXE_DIR%.
) else (
    echo Running %EXE_NAME%...
    start %EXE_DIR%\%EXE_NAME%
)

:: Change back to the original directory
cd %CURRENT_DIR%