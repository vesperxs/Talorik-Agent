@echo off
echo Installing vcpkg and dependencies...

REM Check if vcpkg already exists
if exist "C:\vcpkg" (
    echo vcpkg already exists at C:\vcpkg
    goto :install_packages
)

REM Clone vcpkg
echo Cloning vcpkg...
cd /d C:\
git clone https://github.com/Microsoft/vcpkg.git
if %ERRORLEVEL% neq 0 (
    echo Failed to clone vcpkg. Make sure git is installed.
    pause
    exit /b 1
)

REM Bootstrap vcpkg
echo Bootstrapping vcpkg...
cd vcpkg
call bootstrap-vcpkg.bat
if %ERRORLEVEL% neq 0 (
    echo Failed to bootstrap vcpkg.
    pause
    exit /b 1
)

:install_packages
REM Install required packages
echo Installing required packages...
C:\vcpkg\vcpkg install nlohmann-json crow spdlog asio
if %ERRORLEVEL% neq 0 (
    echo Failed to install packages.
    pause
    exit /b 1
)

echo.
echo vcpkg installation completed successfully!
echo.
echo Now you can build the project with:
echo   scripts/build_windows.bat
echo.
pause 