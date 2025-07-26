@echo off
echo Building TalorikAgent Security Agent (Minimal Version)...

REM Remove build directory if it exists
if exist build (
    echo Removing old build directory...
    rmdir /s /q build
)

REM Create new build directory
echo Creating new build directory...
mkdir build
cd build

REM Configure with CMake (minimal build)
echo Configuring with CMake...
cmake .. -DBUILD_MINIMAL=ON -DCMAKE_BUILD_TYPE=Release

REM Build the project
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% equ 0 (
    echo.
    echo Build completed successfully!
    echo.
    echo To run the agent:
    echo   cd build\bin\Release
    echo   TalorikAgent.exe
    echo.
) else (
    echo.
    echo Build failed!
    echo.
)

pause 