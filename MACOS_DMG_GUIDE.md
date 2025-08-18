# 🍎 macOS DMG Creation Guide

## 📋 Prerequisites
- **macOS System** (Big Sur 11.0+ recommended)
- **Qt 6.9.1+** for macOS
- **Xcode Command Line Tools**: `xcode-select --install`
- **Homebrew** (optional): `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`

## 🛠️ Installation Steps

### **1. Install Dependencies**
```bash
# Install Qt via Homebrew (easier)
brew install qt

# OR download Qt installer from qt.io
# https://www.qt.io/download-qt-installer

# Install create-dmg for enhanced DMG (optional)
brew install create-dmg
```

### **2. Clone Repository**
```bash
git clone https://github.com/abdurrhmansaeed/ColorRemover.git
cd ColorRemover
```

### **3. Build DMG**
```bash
# Method A: Simple build (already included)
chmod +x build_macos.sh
./build_macos.sh

# Method B: Enhanced build (better DMG)
chmod +x build_macos_enhanced.sh
./build_macos_enhanced.sh
```

## 📦 What Gets Created

### **🗂️ File Structure After Build:**
```
build_macos/
├── ColorRemover.app/          # macOS app bundle
│   ├── Contents/
│   │   ├── MacOS/ColorRemover # Executable
│   │   ├── Resources/         # Icons, translations
│   │   ├── Frameworks/        # Qt frameworks
│   │   └── Info.plist         # App metadata
├── ColorRemover_v1.0_macOS.dmg # DMG installer
└── ColorRemover_v1.0_macOS_Enhanced.dmg # Enhanced DMG (if create-dmg available)
```

### **💿 DMG Contents:**
- **ColorRemover.app** - Drag to Applications
- **Applications** - Symlink to /Applications folder
- **README.txt** - Installation instructions
- **Background image** (enhanced version)

## 🎯 DMG Features

### **✅ Standard DMG Includes:**
- Professional app bundle with Qt frameworks
- Drag-to-Applications installation
- Proper code signing (when certificates available)
- Compressed format for smaller file size
- Compatible with macOS 10.15+

### **🎨 Enhanced DMG Includes:**
- Custom background image
- Positioned app and Applications icons
- Custom window size and position
- Volume icon customization
- Professional installer appearance

## 🔧 Manual DMG Creation (Alternative)

If automated scripts fail, create DMG manually:

### **Step 1: Build App Bundle**
```bash
qmake ColorRemover.pro CONFIG+=release
make
macdeployqt ColorRemover.app
```

### **Step 2: Create DMG Folder**
```bash
mkdir dmg_temp
cp -R ColorRemover.app dmg_temp/
ln -s /Applications dmg_temp/Applications
```

### **Step 3: Create DMG**
```bash
hdiutil create -volname "ColorRemover v1.0" \
               -srcfolder dmg_temp \
               -ov \
               -format UDZO \
               ColorRemover_v1.0_macOS.dmg
```

## 📋 DMG Size Expectations

| **Component** | **Size** | **Description** |
|---------------|----------|-----------------|
| **App Bundle** | ~15-25 MB | App + Qt frameworks |
| **DMG File** | ~8-15 MB | Compressed installer |
| **Uncompressed** | ~30-40 MB | Full installation size |

## ✅ Testing Your DMG

### **🧪 Quality Checklist:**
```bash
# 1. Mount DMG
hdiutil attach ColorRemover_v1.0_macOS.dmg

# 2. Test drag-to-Applications
cp "/Volumes/ColorRemover v1.0/ColorRemover.app" /Applications/

# 3. Launch app
open /Applications/ColorRemover.app

# 4. Test functionality
# - Open image
# - Remove color
# - Switch languages
# - Save image

# 5. Unmount DMG
hdiutil detach "/Volumes/ColorRemover v1.0"
```

## 🚀 Distribution

### **📤 Upload to GitHub Releases:**
- File: `ColorRemover_v1.0_macOS.dmg`
- Size: ~8-15 MB
- Compatible: macOS 10.15+

### **🏪 Mac App Store (Optional):**
- Requires Apple Developer account ($99/year)
- Additional code signing requirements
- App Store review process

## 🔧 Troubleshooting

### **❌ Common Issues:**

#### **"Qt not found"**
```bash
# Add Qt to PATH
export PATH="/opt/homebrew/bin:$PATH"
# OR
export PATH="/usr/local/opt/qt/bin:$PATH"
```

#### **"macdeployqt failed"**
```bash
# Use full path
/path/to/qt/bin/macdeployqt ColorRemover.app
```

#### **"DMG mount failed"**
```bash
# Check DMG integrity
hdiutil verify ColorRemover_v1.0_macOS.dmg
```

#### **"App won't launch"**
```bash
# Check app bundle structure
ls -la ColorRemover.app/Contents/MacOS/
# Verify executable permissions
chmod +x ColorRemover.app/Contents/MacOS/ColorRemover
```

## 💡 Pro Tips

1. **Test on clean macOS** - Different versions, fresh installs
2. **Code signing** - Use Developer ID for distribution outside App Store
3. **Notarization** - Required for macOS 10.15+ (Catalina)
4. **Universal binaries** - Build for both Intel and Apple Silicon
5. **App Store guidelines** - If targeting Mac App Store

---

**Ready to build your professional macOS DMG installer!** 🍎✨
