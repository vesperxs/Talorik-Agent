@echo off
echo Installing nlohmann-json manually...

if exist C:\vcpkg\vcpkg.exe (
    echo Installing nlohmann-json...
    C:\vcpkg\vcpkg.exe install nlohmann-json:x64-windows
    
    if %ERRORLEVEL% equ 0 (
        echo nlohmann-json installed successfully!
        echo.
        echo Now try building again with: scripts/build_available.bat
    ) else (
        echo Failed to install nlohmann-json
        echo.
        echo You can try the minimal build instead: scripts/build_minimal.bat
    )
) else (
    echo vcpkg not found at C:\vcpkg\vcpkg.exe
    echo Please install vcpkg first
)

echo.
pause 