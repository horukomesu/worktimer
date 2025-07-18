@echo off
echo Building WorkTimer C++ version...

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure with CMake for Visual Studio 2022
echo Configuring with CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo Executable is located at: build\bin\Release\WorkTimer.exe
echo.

REM Copy sounds folder to output directory
if not exist "bin\Release\sounds" mkdir bin\Release\sounds
xcopy /Y /E "..\sounds\*" "bin\Release\sounds\"

echo Sounds folder copied to output directory.
echo.
echo Ready to run: build\bin\Release\WorkTimer.exe
pause 