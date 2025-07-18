@echo off
echo Cleaning Qt dependencies with minimal set for sound functionality...

REM Check if Release directory exists
if not exist "build\bin\Release" (
    echo Error: Release directory not found!
    echo Please run build_cpp.bat first
    exit /b 1
)

echo Removing unnecessary Qt dependencies...

REM Force close any processes that might be using the files
taskkill /F /IM WorkTimer.exe 2>nul

REM Wait a moment for files to be released
timeout /t 2 /nobreak >nul

REM Remove only unnecessary DLL files (keep all audio-related)
if exist "build\bin\Release\Qt6Svg.dll" del /F "build\bin\Release\Qt6Svg.dll"

REM Remove unnecessary directories but keep multimedia
if exist "build\bin\Release\imageformats" rmdir /S /Q "build\bin\Release\imageformats"
if exist "build\bin\Release\iconengines" rmdir /S /Q "build\bin\Release\iconengines"
if exist "build\bin\Release\networkinformation" rmdir /S /Q "build\bin\Release\networkinformation"
if exist "build\bin\Release\generic" rmdir /S /Q "build\bin\Release\generic"
if exist "build\bin\Release\tls" rmdir /S /Q "build\bin\Release\tls"

echo.
echo Minimal cleanup completed!
echo.
echo Remaining files in Release directory:
dir /B "build\bin\Release\*.dll"
echo.
echo Total size after cleanup:
dir "build\bin\Release" | find "File(s)" 