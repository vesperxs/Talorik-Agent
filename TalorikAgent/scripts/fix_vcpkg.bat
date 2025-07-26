@echo off
echo Fixing vcpkg package issues...

if not exist C:\vcpkg\vcpkg.exe (
    echo vcpkg not found at C:\vcpkg\vcpkg.exe
    echo Please install vcpkg first
    pause
    exit /b 1
)

echo Updating vcpkg...
C:\vcpkg\vcpkg.exe update

echo Installing packages individually...

echo Installing nlohmann-json...
C:\vcpkg\vcpkg.exe install nlohmann-json:x64-windows
if %ERRORLEVEL% neq 0 (
    echo Failed to install nlohmann-json
    goto :try_minimal
)

echo Installing asio...
C:\vcpkg\vcpkg.exe install asio:x64-windows
if %ERRORLEVEL% neq 0 (
    echo Failed to install asio
    goto :try_minimal
)

echo All packages installed successfully!
echo Now try: scripts/build_windows.bat
goto :end

:try_minimal
echo.
echo Some packages failed to install.
echo Trying minimal build instead...
echo.
call scripts\build_minimal.bat
goto :end

:end
pause 