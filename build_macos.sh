#!/bin/bash
# ColorRemover macOS Build Script
# Repository: https://github.com/abdurrhmansaeed/ColorRemover

echo "=== ColorRemover macOS Build ==="

# Check if Qt is installed
if ! command -v qmake &> /dev/null; then
    echo "❌ Qt not found. Please install Qt:"
    echo "   brew install qt"
    echo "   Or download from: https://www.qt.io/download"
    exit 1
fi

# Clean previous builds
echo "🧹 Cleaning previous builds..."
make clean 2>/dev/null || true
rm -rf build_macos/
mkdir -p build_macos/

# Build the application
echo "🔨 Building ColorRemover for macOS..."
cd build_macos/
qmake ../ColorRemover.pro CONFIG+=release
make -j$(sysctl -n hw.ncpu)

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    
    # Create app bundle
    echo "📦 Creating app bundle..."
    macdeployqt ColorRemover.app
    
    # Copy translations into bundle
    mkdir -p ColorRemover.app/Contents/Resources/translations/
    cp ../translations/*.json ColorRemover.app/Contents/Resources/translations/
    
    # Create DMG
    echo "💿 Creating DMG installer..."
    hdiutil create -volname "ColorRemover" -srcfolder ColorRemover.app -ov -format UDZO ColorRemover.dmg
    
    echo "🎉 ColorRemover.app and ColorRemover.dmg created successfully!"
    echo "📁 Location: build_macos/"
    echo "🔗 Repository: https://github.com/abdurrhmansaeed/ColorRemover"
else
    echo "❌ Build failed!"
    exit 1
fi
