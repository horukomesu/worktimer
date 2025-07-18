@echo off
echo Deploying Qt dependencies with minimal set...

REM Check if build exists
if not exist "build\bin\Release\WorkTimer.exe" (
    echo Error: WorkTimer.exe not found!
    echo Please build the project first using build_cpp.bat
    exit /b 1
)

REM Find windeployqt
set "WINDEPLOYQT_PATH="

REM Try to find windeployqt in Qt installation
if exist "C:\Qt\6.9.1\msvc2022_64\bin\windeployqt.exe" (
    set "WINDEPLOYQT_PATH=C:\Qt\6.9.1\msvc2022_64\bin\windeployqt.exe"
) else if exist "C:\Qt\6.9.1\mingw_64\bin\windeployqt.exe" (
    set "WINDEPLOYQT_PATH=C:\Qt\6.9.1\mingw_64\bin\windeployqt.exe"
) else (
    echo Error: windeployqt not found!
    echo Please check your Qt installation.
    exit /b 1
)

echo Found windeployqt at: %WINDEPLOYQT_PATH%
echo.

REM Clean up any existing DLL files in Release directory
echo Cleaning up existing DLL files in Release directory...
if exist "build\bin\Release\*.dll" del /Q "build\bin\Release\*.dll"
if exist "build\bin\Release\platforms" rmdir /S /Q "build\bin\Release\platforms"
if exist "build\bin\Release\imageformats" rmdir /S /Q "build\bin\Release\imageformats"
if exist "build\bin\Release\iconengines" rmdir /S /Q "build\bin\Release\iconengines"
if exist "build\bin\Release\styles" rmdir /S /Q "build\bin\Release\styles"
if exist "build\bin\Release\multimedia" rmdir /S /Q "build\bin\Release\multimedia"
if exist "build\bin\Release\networkinformation" rmdir /S /Q "build\bin\Release\networkinformation"
if exist "build\bin\Release\generic" rmdir /S /Q "build\bin\Release\generic"
if exist "build\bin\Release\tls" rmdir /S /Q "build\bin\Release\tls"

REM Deploy Qt dependencies with minimal set
echo Deploying minimal Qt dependencies to Release directory...
"%WINDEPLOYQT_PATH%" ^
    --no-compiler-runtime ^
    --no-translations ^
    --no-system-d3d-compiler ^
    --no-opengl-sw ^
    --dir "build\bin\Release" ^
    "build\bin\Release\WorkTimer.exe"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Minimal Qt dependencies deployed successfully to Release directory!
    echo.
    echo Files in Release directory:
    dir /B "build\bin\Release\*.dll"
    echo.
    echo Total size of deployment:
    dir "build\bin\Release" | find "File(s)"
) else (
    echo.
    echo Error: Failed to deploy Qt dependencies!
) 