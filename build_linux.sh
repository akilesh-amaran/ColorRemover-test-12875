#!/bin/bash
# ColorRemover Linux Build Script
# Repository: https://github.com/abdurrhmansaeed/ColorRemover

echo "=== ColorRemover Linux Build ==="

# Check if Qt is installed
if ! command -v qmake &> /dev/null; then
    echo "❌ Qt not found. Please install Qt development tools:"
    echo "   Ubuntu/Debian: sudo apt-get install qt6-base-dev qt6-tools-dev"
    echo "   Fedora: sudo dnf install qt6-qtbase-devel qt6-qttools-devel"
    echo "   Arch: sudo pacman -S qt6-base qt6-tools"
    exit 1
fi

# Clean previous builds
echo "🧹 Cleaning previous builds..."
make clean 2>/dev/null || true
rm -rf build_linux/
mkdir -p build_linux/

# Build the application
echo "🔨 Building ColorRemover for Linux..."
cd build_linux/
qmake ../ColorRemover.pro CONFIG+=release
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo "📦 Executable: build_linux/ColorRemover"
    
    # Copy translations
    mkdir -p translations/
    cp ../translations/*.json translations/
    
    # Create AppImage (optional)
    if command -v linuxdeploy-x86_64.AppImage &> /dev/null; then
        echo "📱 Creating AppImage..."
        linuxdeploy-x86_64.AppImage --appdir AppDir --executable ColorRemover --desktop-file ../linux/ColorRemover.desktop --icon-file ../icons/app_icon.png --output appimage
    fi
    
    echo "🎉 ColorRemover Linux build complete!"
    echo "🔗 Repository: https://github.com/abdurrhmansaeed/ColorRemover"
else
    echo "❌ Build failed!"
    exit 1
fi
