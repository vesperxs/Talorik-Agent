@echo off
echo Building TalorikAgent Security Agent (No vcpkg)...

REM Change to project root directory
cd /d "%~dp0.."

REM Remove old build directory if it exists
if exist build (
    echo Removing old build directory...
    rmdir /s /q build
)

REM Create build directory
echo Creating new build directory...
mkdir build
cd build

REM Configure with CMake (no vcpkg, minimal build)
echo Configuring with CMake (no vcpkg)...
cmake .. -DBUILD_MINIMAL=ON -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo.
echo To run the security agent:
echo   cd build\bin\Release
echo   TalorikAgent.exe
echo.
pause 