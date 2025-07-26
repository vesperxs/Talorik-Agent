@echo off
echo ========================================
echo TalorikAgent Security Agent Compiler
echo ========================================
echo.

echo Testing available compilers...

REM Try MSVC (Visual Studio)
echo [1/4] Trying MSVC (Visual Studio)...
where cl >nul 2>&1
if %errorlevel% equ 0 (
    echo Found MSVC compiler
    echo Compiling with MSVC...
    cl /EHsc /std:c++17 build_simple.cpp ws2_32.lib /Fe:talorik_agent.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo SUCCESS: Compiled with MSVC!
        echo ========================================
        echo.
        echo Running the agent...
        echo The agent will start an HTTP server on port 8080
        echo You can access JSON messages at: http://localhost:8080/
        echo Press Ctrl+C to stop the agent
        echo.
        talorik_agent.exe
        goto :end
    ) else (
        echo MSVC compilation failed
    )
) else (
    echo MSVC not found
)

REM Try MinGW-w64
echo [2/4] Trying MinGW-w64...
where g++ >nul 2>&1
if %errorlevel% equ 0 (
    echo Found MinGW-w64 compiler
    echo Compiling with MinGW-w64...
    g++ -std=c++17 -O2 build_simple.cpp -lws2_32 -o talorik_agent.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo SUCCESS: Compiled with MinGW-w64!
        echo ========================================
        echo.
        echo Running the agent...
        echo The agent will start an HTTP server on port 8080
        echo You can access JSON messages at: http://localhost:8080/
        echo Press Ctrl+C to stop the agent
        echo.
        talorik_agent.exe
        goto :end
    ) else (
        echo MinGW-w64 compilation failed
    )
) else (
    echo MinGW-w64 not found
)

REM Try Clang
echo [3/4] Trying Clang...
where clang++ >nul 2>&1
if %errorlevel% equ 0 (
    echo Found Clang compiler
    echo Compiling with Clang...
    clang++ -std=c++17 -O2 build_simple.cpp -lws2_32 -o talorik_agent.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo SUCCESS: Compiled with Clang!
        echo ========================================
        echo.
        echo Running the agent...
        echo The agent will start an HTTP server on port 8080
        echo You can access JSON messages at: http://localhost:8080/
        echo Press Ctrl+C to stop the agent
        echo.
        talorik_agent.exe
        goto :end
    ) else (
        echo Clang compilation failed
    )
) else (
    echo Clang not found
)

REM Try MSVC with vcvars64
echo [4/4] Trying MSVC with vcvars64...
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    echo Found Visual Studio 2022 Community
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    cl /EHsc /std:c++17 build_simple.cpp ws2_32.lib /Fe:talorik_agent.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo SUCCESS: Compiled with MSVC (vcvars64)!
        echo ========================================
        echo.
        echo Running the agent...
        echo The agent will start an HTTP server on port 8080
        echo You can access JSON messages at: http://localhost:8080/
        echo Press Ctrl+C to stop the agent
        echo.
        talorik_agent.exe
        goto :end
    )
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" (
    echo Found Visual Studio 2022 Professional
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    cl /EHsc /std:c++17 build_simple.cpp ws2_32.lib /Fe:talorik_agent.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo SUCCESS: Compiled with MSVC (vcvars64)!
        echo ========================================
        echo.
        echo Running the agent...
        echo The agent will start an HTTP server on port 8080
        echo You can access JSON messages at: http://localhost:8080/
        echo Press Ctrl+C to stop the agent
        echo.
        talorik_agent.exe
        goto :end
    )
) else if exist "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat" (
    echo Found Visual Studio 2022 Enterprise
    call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    cl /EHsc /std:c++17 build_simple.cpp ws2_32.lib /Fe:talorik_agent.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo SUCCESS: Compiled with MSVC (vcvars64)!
        echo ========================================
        echo.
        echo Running the agent...
        echo The agent will start an HTTP server on port 8080
        echo You can access JSON messages at: http://localhost:8080/
        echo Press Ctrl+C to stop the agent
        echo.
        talorik_agent.exe
        goto :end
    )
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat" (
    echo Found Visual Studio 2019 Community
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
    cl /EHsc /std:c++17 build_simple.cpp ws2_32.lib /Fe:talorik_agent.exe
    if %errorlevel% equ 0 (
        echo.
        echo ========================================
        echo SUCCESS: Compiled with MSVC (vcvars64)!
        echo ========================================
        echo.
        echo Running the agent...
        echo The agent will start an HTTP server on port 8080
        echo You can access JSON messages at: http://localhost:8080/
        echo Press Ctrl+C to stop the agent
        echo.
        talorik_agent.exe
        goto :end
    )
) else (
    echo Visual Studio not found in standard locations
)

echo.
echo ========================================
echo ERROR: No suitable compiler found!
echo ========================================
echo.
echo Please install one of the following:
echo 1. Visual Studio 2019/2022 (Community/Professional/Enterprise)
echo 2. MinGW-w64
echo 3. Clang
echo.
echo Or run the PowerShell script: compile.ps1
echo.

:end
pause 