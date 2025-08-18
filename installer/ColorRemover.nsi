; ColorRemover NSIS Installer Script
!define APPNAME "ColorRemover"
!define APPVERSION "1.0"
!define DESCRIPTION "Professional Image Color Removal Tool"
!define COMPANYNAME "Abdurrhman Saeed"
!define VERSIONMAJOR 1
!define VERSIONMINOR 0

Name "${APPNAME}"
OutFile "ColorRemover_v1.0_Setup.exe"
InstallDir "$PROGRAMFILES\ColorRemover"
RequestExecutionLevel admin

; Modern UI
!include "MUI2.nsh"

; Pages
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

Section "ColorRemover" SecMain
    SetOutPath $INSTDIR
    File "ColorRemover.exe"
    File "README.md"
    File "LICENSE"
    
    SetOutPath $INSTDIR\translations
    File /r "translations\*"
    
    ; Create Start Menu shortcuts
    CreateDirectory "$SMPROGRAMS\ColorRemover"
    CreateShortCut "$SMPROGRAMS\ColorRemover\ColorRemover.lnk" "$INSTDIR\ColorRemover.exe"
    CreateShortCut "$SMPROGRAMS\ColorRemover\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    
    ; Create Desktop shortcut
    CreateShortCut "$DESKTOP\ColorRemover.lnk" "$INSTDIR\ColorRemover.exe"
    
    ; Registry entries for Add/Remove Programs
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "DisplayName" "${APPNAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "DisplayVersion" "${APPVERSION}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "Publisher" "${COMPANYNAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "DisplayIcon" "$INSTDIR\ColorRemover.exe"
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "VersionMajor" ${VERSIONMAJOR}
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "VersionMinor" ${VERSIONMINOR}
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover" "NoRepair" 1
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
    Delete "$INSTDIR\ColorRemover.exe"
    Delete "$INSTDIR\README.md"
    Delete "$INSTDIR\LICENSE"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir /r "$INSTDIR\translations"
    RMDir "$INSTDIR"
    
    Delete "$DESKTOP\ColorRemover.lnk"
    Delete "$SMPROGRAMS\ColorRemover\ColorRemover.lnk"
    Delete "$SMPROGRAMS\ColorRemover\Uninstall.lnk"
    RMDir "$SMPROGRAMS\ColorRemover"
    
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ColorRemover"
SectionEnd
