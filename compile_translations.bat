@echo off
echo Compiling translation files...

set QT_DIR=C:\Qt\6.9.1\msvc2022_64

echo Using Qt from: %QT_DIR%

REM Compile Russian translation
echo Compiling Russian translation...
"%QT_DIR%\bin\lrelease.exe" translations\worktimer_ru.ts -qm translations\worktimer_ru.qm

REM Compile English translation
echo Compiling English translation...
"%QT_DIR%\bin\lrelease.exe" translations\worktimer_en.ts -qm translations\worktimer_en.qm

echo Translation compilation completed!
pause 