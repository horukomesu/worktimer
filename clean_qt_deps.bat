@echo off
echo Cleaning unnecessary Qt dependencies...

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

REM Remove unnecessary DLL files (keep audio libraries for sound)
if exist "build\bin\Release\Qt6Svg.dll" del /F "build\bin\Release\Qt6Svg.dll"

REM Keep audio libraries for sound functionality
REM if exist "build\bin\Release\avcodec-61.dll" del /F "build\bin\Release\avcodec-61.dll"
REM if exist "build\bin\Release\avformat-61.dll" del /F "build\bin\Release\avformat-61.dll"
REM if exist "build\bin\Release\avutil-59.dll" del /F "build\bin\Release\avutil-59.dll"
REM if exist "build\bin\Release\swresample-5.dll" del /F "build\bin\Release\swresample-5.dll"
REM if exist "build\bin\Release\swscale-8.dll" del /F "build\bin\Release\swscale-8.dll"

REM Remove unnecessary directories
if exist "build\bin\Release\imageformats" rmdir /S /Q "build\bin\Release\imageformats"
if exist "build\bin\Release\iconengines" rmdir /S /Q "build\bin\Release\iconengines"
if exist "build\bin\Release\networkinformation" rmdir /S /Q "build\bin\Release\networkinformation"
if exist "build\bin\Release\generic" rmdir /S /Q "build\bin\Release\generic"
if exist "build\bin\Release\tls" rmdir /S /Q "build\bin\Release\tls"

echo.
echo Cleanup completed!
echo.
echo Remaining files in Release directory:
dir /B "build\bin\Release\*.dll"
echo.
echo Total size after cleanup:
dir "build\bin\Release" | find "File(s)" 