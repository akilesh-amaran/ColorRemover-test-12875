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
echo [1/4] Building ColorRemover...
call build.bat
if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

REM Create installer directory
echo [2/4] Preparing installer files...
if exist "installer" rmdir /s /q "installer"
mkdir "installer"

REM Copy files for installer
copy "release\ColorRemover.exe" "installer\"
xcopy "translations" "installer\translations\" /E /Y /I >nul
copy "README.md" "installer\"
copy "LICENSE" "installer\"

REM Create NSIS script
echo [3/4] Creating NSIS installer script...
(
echo ; ColorRemover NSIS Installer Script
echo !define APPNAME "ColorRemover"
echo !define APPVERSION "1.0"
echo !define DESCRIPTION "Professional Image Color Removal Tool"
echo !define COMPANYNAME "Abdurrhman Saeed"
echo !define VERSIONMAJOR 1
echo !define VERSIONMINOR 0
echo.
echo OutFile "ColorRemover_v1.0_Setup.exe"
echo InstallDir "$PROGRAMFILES\ColorRemover"
echo RequestExecutionLevel admin
echo.
echo Page directory
echo Page instfiles
echo.
echo Section ""
echo     SetOutPath $INSTDIR
echo     File "ColorRemover.exe"
echo     File "README.md"
echo     File "LICENSE"
echo     SetOutPath $INSTDIR\translations
echo     File /r "translations\*"
echo     
echo     ; Create Start Menu shortcuts
echo     CreateDirectory "$SMPROGRAMS\ColorRemover"
echo     CreateShortCut "$SMPROGRAMS\ColorRemover\ColorRemover.lnk" "$INSTDIR\ColorRemover.exe"
echo     CreateShortCut "$SMPROGRAMS\ColorRemover\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
echo     
echo     ; Create Desktop shortcut
echo     CreateShortCut "$DESKTOP\ColorRemover.lnk" "$INSTDIR\ColorRemover.exe"
echo     
echo     ; Registry entries
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "DisplayName" "ColorRemover"
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "UninstallString" "$INSTDIR\Uninstall.exe"
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "DisplayVersion" "1.0"
echo     WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "Publisher" "Abdurrhman Saeed"
echo     
echo     ; Create uninstaller
echo     WriteUninstaller "$INSTDIR\Uninstall.exe"
echo SectionEnd
echo.
echo Section "Uninstall"
echo     Delete "$INSTDIR\ColorRemover.exe"
echo     Delete "$INSTDIR\README.md" 
echo     Delete "$INSTDIR\LICENSE"
echo     Delete "$INSTDIR\Uninstall.exe"
echo     RMDir /r "$INSTDIR\translations"
echo     RMDir "$INSTDIR"
echo     
echo     Delete "$DESKTOP\ColorRemover.lnk"
echo     Delete "$SMPROGRAMS\ColorRemover\ColorRemover.lnk"
echo     Delete "$SMPROGRAMS\ColorRemover\Uninstall.lnk"
echo     RMDir "$SMPROGRAMS\ColorRemover"
echo     
echo     DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover"
echo SectionEnd
) > "installer\ColorRemover.nsi"

REM Build installer
echo [4/4] Building installer...
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
