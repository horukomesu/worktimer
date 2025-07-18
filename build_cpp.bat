@echo off
echo Building WorkTimer C++ version with optimized Qt deployment...

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
echo.

REM Return to root directory for deployment
cd ..

REM Run optimized Qt deployment to Release directory
echo Running optimized Qt deployment...
call deploy_qt_optimized.bat

if %ERRORLEVEL% NEQ 0 (
    echo Qt deployment failed!
    pause
    exit /b 1
)

REM Clean up unnecessary Qt dependencies
echo Cleaning up unnecessary Qt dependencies...
call clean_qt_deps.bat

REM Clean up bin directory (remove DLL files, keep only exe)
echo Cleaning up bin directory...
call clean_bin.bat

echo.
echo Executable is located at: build\bin\Release\WorkTimer.exe
echo.

REM Copy sounds folder to Release directory (if not already copied)
if not exist "build\bin\Release\sounds" mkdir build\bin\Release\sounds
xcopy /Y /E "sounds\*" "build\bin\Release\sounds\"

echo Sounds folder copied to Release directory.
echo.
echo Ready to run: build\bin\Release\WorkTimer.exe
echo.
echo Note: This is now a standalone executable with minimal Qt dependencies in Release directory!
pause 