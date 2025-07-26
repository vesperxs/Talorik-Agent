@echo off
echo Checking available vcpkg packages...

if exist C:\vcpkg\vcpkg.exe (
    echo vcpkg found at C:\vcpkg\vcpkg.exe
    echo.
    echo Checking installed packages:
    C:\vcpkg\vcpkg.exe list
    echo.
    echo Checking available packages:
    C:\vcpkg\vcpkg.exe search nlohmann-json
    C:\vcpkg\vcpkg.exe search asio
    C:\vcpkg\vcpkg.exe search gtest
) else (
    echo vcpkg not found at C:\vcpkg\vcpkg.exe
    echo Please install vcpkg first
)

echo.
echo Package check completed.
pause 