@echo off
echo Building WorkTimer Installer...

REM Check if the executable exists in Release directory
if not exist "build\bin\Release\WorkTimer.exe" (
    echo Error: WorkTimer.exe not found in build\bin\Release!
    echo Please build the project first using build_cpp.bat
    exit /b 1
)

REM Check if Qt dependencies are deployed
if not exist "build\bin\Release\Qt6Core.dll" (
    echo Warning: Qt dependencies not found in Release directory!
    echo Please run deploy_qt.bat first
    exit /b 1
)

REM Try to find Inno Setup Compiler
set "ISCC_PATH="

if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" (
    set "ISCC_PATH=C:\Program Files (x86)\Inno Setup 6\ISCC.exe"
) else if exist "C:\Program Files\Inno Setup 6\ISCC.exe" (
    set "ISCC_PATH=C:\Program Files\Inno Setup 6\ISCC.exe"
) else (
    echo Inno Setup Compiler not found!
    echo Please install Inno Setup 6 from: https://jrsoftware.org/isinfo.php
    exit /b 1
)

echo Found Inno Setup at: %ISCC_PATH%

REM Build the installer
"%ISCC_PATH%" installer.iss

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Installer built successfully!
    echo Check the 'installer' folder for WorkTimerSetup.exe
) else (
    echo.
    echo Failed to build installer!
) 