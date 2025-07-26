@echo off
echo Building TalorikAgent Security Agent (Simple Build)...

REM Remove build directory if it exists
if exist build (
    echo Removing old build directory...
    rmdir /s /q build
)

REM Create new build directory
echo Creating new build directory...
mkdir build
cd build

REM Configure with CMake (without vcpkg)
echo Configuring with CMake...
cmake .. -DBUILD_MINIMAL=ON -DCMAKE_BUILD_TYPE=Release

REM Build the project
echo Building project...
cmake --build . --config Release

echo Build completed!
pause 