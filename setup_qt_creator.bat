@echo off
echo Setting up Qt Creator project configuration...

REM Check if Qt 6.9.1 is installed
if not exist "C:\Qt\6.9.1\msvc2022_64\bin\qmake.exe" (
    echo ERROR: Qt 6.9.1 not found at C:\Qt\6.9.1\msvc2022_64
    echo Please install Qt 6.9.1 with MSVC 2022 support
    pause
    exit /b 1
)

echo Qt 6.9.1 found successfully!

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Configure CMake project
echo Configuring CMake project...
cmake -B build -S . -G "Ninja" -DCMAKE_BUILD_TYPE=Debug

if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo Qt Creator setup completed successfully!
echo ========================================
echo.
echo Next steps:
echo 1. Open Qt Creator 11
echo 2. File -^> Open File or Project
echo 3. Select CMakeLists.txt from this folder
echo 4. Choose MSVC 2022 64bit Kit
echo 5. Press Ctrl+B to build
echo.
echo For detailed instructions, see QT_CREATOR_SETUP.md
echo.
pause 