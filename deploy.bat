@echo off
echo ========================================
echo   ColorRemover v1.0 Deployment Script
echo ========================================
echo.

REM Check if release build exists
if not exist "release\ColorRemover.exe" (
    echo ERROR: Release build not found!
    echo Please run build.bat first.
    pause
    exit /b 1
)

echo [1/3] Creating deployment directory...
set DEPLOY_DIR=ColorRemover_v1.0_Windows_Portable
if exist "%DEPLOY_DIR%" rmdir /s /q "%DEPLOY_DIR%"
mkdir "%DEPLOY_DIR%"

echo [2/3] Copying application files...
copy "release\ColorRemover.exe" "%DEPLOY_DIR%\"
xcopy "translations" "%DEPLOY_DIR%\translations\" /E /Y /I >nul
copy "README.md" "%DEPLOY_DIR%\"
copy "LICENSE" "%DEPLOY_DIR%\"

echo [3/3] Deploying Qt dependencies...
cd "%DEPLOY_DIR%"
windeployqt ColorRemover.exe --release --no-system-d3d-compiler --no-opengl-sw
cd ..

echo.
echo ✓ DEPLOYMENT SUCCESSFUL!
echo ✓ Portable version: %DEPLOY_DIR%
echo.
echo Ready for distribution!
pause
