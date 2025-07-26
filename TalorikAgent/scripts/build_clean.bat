@echo off
echo Cleaning and building TalorikAgent Security Agent...

REM Remove build directory if it exists
if exist build (
    echo Removing old build directory...
    rmdir /s /q build
)

REM Create new build directory
echo Creating new build directory...
mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

REM Build the project
echo Building project...
cmake --build . --config Release

echo Build completed!
pause 