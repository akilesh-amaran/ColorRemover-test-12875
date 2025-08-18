@echo off
echo ========================================
echo   ColorRemover Windows Installer Creator
echo ========================================
echo.

REM Check if NSIS is installed
set NSIS_PATH="C:\Program Files (x86)\NSIS\makensis.exe"
if not exist %NSIS_PATH% (
    echo ERROR: NSIS not found!
    echo Please install NSIS from: https://nsis.sourceforge.io/
    echo Or check if NSIS is installed in a different location
    pause
    exit /b 1
)

REM Build the application first
echo [1/3] Building ColorRemover...
call build.bat
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

REM Create installer directory and copy files
echo [2/3] Preparing installer files...
if exist "installer" rmdir /s /q "installer"
mkdir "installer"

copy "release\ColorRemover.exe" "installer\"
xcopy "translations" "installer\translations\" /E /Y /I >nul
copy "README.md" "installer\"
copy "LICENSE" "installer\"
copy "ColorRemover.nsi" "installer\"

REM Build installer
echo [3/3] Building installer...
cd installer
%NSIS_PATH% ColorRemover.nsi
if %ERRORLEVEL% neq 0 (
    echo ERROR: Installer creation failed!
    cd ..
    pause
    exit /b 1
)

cd ..
move "installer\ColorRemover_v1.0_Setup.exe" .

echo.
echo ✓ INSTALLER CREATED SUCCESSFULLY!
echo ✓ File: ColorRemover_v1.0_Setup.exe
echo.
echo Ready for distribution!
pause
