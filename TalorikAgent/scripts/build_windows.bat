@echo off
echo Building TalorikAgent Security Agent...

REM Change to project root directory (where CMakeLists.txt is located)
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

REM Try to configure with vcpkg first
echo Configuring with CMake (trying vcpkg)...
if exist "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" (
    cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
    if %ERRORLEVEL% equ 0 (
        echo vcpkg configuration successful
        goto :build
    ) else (
        echo vcpkg configuration failed (likely package install issue)
        echo Trying minimal build without vcpkg...
    )
) else (
    echo vcpkg not found, trying minimal build...
)

REM Try minimal build without vcpkg
echo Configuring with CMake (minimal build)...
cmake .. -DBUILD_MINIMAL=ON -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    echo.
    echo Try running: scripts/build_minimal.bat
    pause
    exit /b 1
)

:build
REM Build the project
echo Building project...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    echo.
    echo Try running: scripts/build_minimal.bat
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
if exist "..\..\scripts\test_api.py" (
    echo To test the API:
    echo   python ..\..\scripts\test_api.py
    echo.
)
pause 