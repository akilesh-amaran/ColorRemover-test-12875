@echo off
echo ========================================
echo    ColorRemover v1.0 Build Script
echo ========================================
echo.

REM Clean previous builds
echo [1/4] Cleaning previous builds...
if exist "release" rmdir /s /q "release"
if exist "debug" rmdir /s /q "debug"
if exist "Makefile*" del /q "Makefile*"
if exist ".qmake.stash" del /q ".qmake.stash"

REM Generate Makefile
echo [2/4] Generating Makefile...
qmake ColorRemover.pro
if %ERRORLEVEL% neq 0 (
    echo ERROR: qmake failed!
    pause
    exit /b 1
)

REM Build the project
echo [3/4] Building ColorRemover...
mingw32-make
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

REM Verify build
echo [4/4] Verifying build...
if exist "release\ColorRemover.exe" (
    echo.
    echo ✓ BUILD SUCCESSFUL!
    echo ✓ Executable: release\ColorRemover.exe
    dir "release\ColorRemover.exe" | findstr ColorRemover
    echo.
    echo Ready for deployment!
) else (
    echo.
    echo ✗ BUILD FAILED!
    echo Executable not found in release directory.
)

echo.
echo Build process complete.
pause
