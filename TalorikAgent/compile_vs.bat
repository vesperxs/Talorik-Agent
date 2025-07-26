@echo off
echo Opening Visual Studio Developer Command Prompt...
echo.
echo This will open a new command prompt with the proper environment.
echo In that prompt, run: cl /std:c++17 build_simple.cpp /Fe:TalorikAgent.exe
echo.
echo Press any key to continue...
pause

REM Try to find and open the Developer Command Prompt
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" (
    echo Found Visual Studio 2022 Developer Command Prompt
    start "Visual Studio 2022 Developer Command Prompt" cmd /k "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat && cd /d \"%~dp0\" && echo. && echo Now run: cl /std:c++17 build_simple.cpp /Fe:TalorikAgent.exe && echo."
) else if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
    echo Found Visual Studio 2019 Developer Command Prompt
    start "Visual Studio 2019 Developer Command Prompt" cmd /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat && cd /d \"%~dp0\" && echo. && echo Now run: cl /std:c++17 build_simple.cpp /Fe:TalorikAgent.exe && echo."
) else (
    echo Visual Studio Developer Command Prompt not found.
    echo.
    echo Please install Visual Studio Community (free) from:
    echo https://visualstudio.microsoft.com/downloads/
    echo.
    echo Or try running: compile_simple.bat
    pause
    exit /b 1
) 