@echo off
echo Cleaning build\bin directory...

REM Check if build\bin exists
if not exist "build\bin" (
    echo Error: build\bin directory not found!
    echo Please build the project first using build_cpp.bat
    pause
    exit /b 1
)

REM Remove all DLL files and Qt-related directories from bin
echo Removing DLL files and Qt directories from bin...
if exist "build\bin\*.dll" del /Q "build\bin\*.dll"
if exist "build\bin\platforms" rmdir /S /Q "build\bin\platforms"
if exist "build\bin\imageformats" rmdir /S /Q "build\bin\imageformats"
if exist "build\bin\iconengines" rmdir /S /Q "build\bin\iconengines"
if exist "build\bin\styles" rmdir /S /Q "build\bin\styles"
if exist "build\bin\multimedia" rmdir /S /Q "build\bin\multimedia"
if exist "build\bin\networkinformation" rmdir /S /Q "build\bin\networkinformation"
if exist "build\bin\generic" rmdir /S /Q "build\bin\generic"
if exist "build\bin\tls" rmdir /S /Q "build\bin\tls"

REM Keep only the exe file and settings
echo Keeping only exe file and settings in bin directory...
echo.

echo Cleanup completed!
echo.
echo Files remaining in build\bin:
dir /B "build\bin"

pause 