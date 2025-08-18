# 📦 ColorRemover Distribution Guide

## 🎯 Available Distribution Packages

### 🖥️ **Desktop Platforms**

#### **🪟 Windows**
- **Source Code**: Build from source using `build.bat`
- **Portable**: Ready-to-run ZIP package
- **Installer**: Full Windows installer with shortcuts and uninstaller

#### **🍎 macOS**  
- **Source Code**: Build from source using `build_macos.sh`
- **DMG**: Drag-and-drop installer package

#### **🐧 Linux**
- **Source Code**: Build from source using `build_linux.sh`
- **AppImage**: Portable single-file executable
- **DEB/RPM**: Distribution-specific packages

### 📱 **Mobile Platforms**

#### **🤖 Android**
- **APK**: Direct install package
- **Google Play**: Store distribution

#### **📱 iOS**
- **IPA**: Enterprise distribution
- **App Store**: Store distribution

---

## 🚀 Quick Start by Platform

### **🪟 Windows Users**

#### Option 1: Full Installer (Recommended)
1. Download `ColorRemover_v1.0_Setup.exe`
2. Run installer as Administrator
3. Follow installation wizard
4. Launch from Start Menu or Desktop

#### Option 2: Portable Version
1. Download `ColorRemover_Windows_Portable.zip`
2. Extract to desired folder
3. Run `ColorRemover.exe`

### **🍎 macOS Users**

1. Download `ColorRemover.dmg`
2. Open DMG file
3. Drag ColorRemover to Applications folder
4. Launch from Applications or Launchpad

### **🐧 Linux Users**

#### Option 1: AppImage (Universal)
```bash
# Download and run
chmod +x ColorRemover-x86_64.AppImage
./ColorRemover-x86_64.AppImage
```

#### Option 2: Build from Source
```bash
# Install dependencies
sudo apt-get install qt6-base-dev qt6-tools-dev  # Ubuntu/Debian
sudo dnf install qt6-qtbase-devel qt6-qttools-devel  # Fedora
sudo pacman -S qt6-base qt6-tools  # Arch

# Build
git clone https://github.com/abdurrhmansaeed/ColorRemover.git
cd ColorRemover
chmod +x build_linux.sh
./build_linux.sh
```

### **📱 Mobile Users**

#### **🤖 Android**
1. Enable "Unknown Sources" in Settings
2. Download `ColorRemover-Android.apk`
3. Install APK file

#### **📱 iOS**
- Install from App Store (coming soon)
- Enterprise distribution requires profile

---

## 🛠️ Building from Source

### **Prerequisites**

#### **All Platforms**
- Qt 6.9.1 or later
- Git for version control
- C++ compiler (GCC, Clang, or MSVC)

#### **Platform-Specific**
- **Windows**: MinGW or Visual Studio
- **macOS**: Xcode Command Line Tools
- **Linux**: GCC and development packages
- **Android**: Android SDK + NDK, JDK 11+
- **iOS**: macOS + Xcode + iOS SDK

### **Build Commands**

```bash
# Clone repository
git clone https://github.com/abdurrhmansaeed/ColorRemover.git
cd ColorRemover

# Windows
build.bat

# macOS
chmod +x build_macos.sh && ./build_macos.sh

# Linux  
chmod +x build_linux.sh && ./build_linux.sh

# Android
chmod +x build_android.sh && ./build_android.sh

# iOS
chmod +x build_ios.sh && ./build_ios.sh
```

---

## 📦 Creating Distribution Packages

### **🪟 Windows Installer**
```bash
# Requires NSIS installer
create_installer.bat
```

### **🍎 macOS DMG**
```bash
# Included in build_macos.sh
./build_macos.sh
```

### **📱 Mobile Apps**
- **Android**: APK generated automatically
- **iOS**: Requires Apple Developer account

---

## 📋 System Requirements

### **🖥️ Desktop**
- **Windows**: 10/11 (64-bit)
- **macOS**: 10.15+ (Catalina or later)
- **Linux**: Any modern distribution with Qt 6 support
- **RAM**: 512 MB minimum, 1 GB recommended
- **Storage**: 50 MB free space
- **Graphics**: OpenGL 2.0 support

### **📱 Mobile**
- **Android**: API level 23+ (Android 6.0+)
- **iOS**: iOS 12.0+ 
- **RAM**: 1 GB minimum
- **Storage**: 25 MB free space

---

## 🔗 Download Links

### **🏠 Official Repository**
**GitHub**: https://github.com/abdurrhmansaeed/ColorRemover

### **📦 Release Downloads**
- **Windows Installer**: `ColorRemover_v1.0_Setup.exe`
- **Windows Portable**: `ColorRemover_Windows_Portable.zip`
- **macOS DMG**: `ColorRemover.dmg`
- **Linux AppImage**: `ColorRemover-x86_64.AppImage`
- **Android APK**: `ColorRemover-Android.apk`

### **🏪 Store Links**
- **Google Play**: Coming Soon
- **App Store**: Coming Soon
- **Microsoft Store**: Coming Soon

---

## 📞 Support

- **🐛 Bug Reports**: [GitHub Issues](https://github.com/abdurrhmansaeed/ColorRemover/issues)
- **💡 Feature Requests**: [GitHub Discussions](https://github.com/abdurrhmansaeed/ColorRemover/discussions)
- **📚 Documentation**: [Wiki](https://github.com/abdurrhmansaeed/ColorRemover/wiki)

---

**ColorRemover v1.0** - Professional Image Color Removal Tool  
**Developer**: Abdurrhman Saeed  
**License**: MIT License
