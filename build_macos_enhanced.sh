#!/bin/bash
# ColorRemover macOS Enhanced Build Script
# Repository: https://github.com/abdurrhmansaeed/ColorRemover

echo "=== ColorRemover macOS Enhanced Build ==="

# Check if Qt is installed
if ! command -v qmake &> /dev/null; then
    echo "❌ Qt not found. Please install Qt:"
    echo "   brew install qt"
    echo "   Or download from: https://www.qt.io/download"
    exit 1
fi

# Check if running on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "❌ This script must be run on macOS"
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
    macdeployqt ColorRemover.app -verbose=2
    
    # Copy translations into bundle
    echo "🌍 Adding translations..."
    mkdir -p ColorRemover.app/Contents/Resources/translations/
    cp ../translations/*.json ColorRemover.app/Contents/Resources/translations/
    
    # Copy additional resources
    echo "📋 Adding documentation..."
    cp ../README.md ColorRemover.app/Contents/Resources/
    cp ../LICENSE ColorRemover.app/Contents/Resources/
    
    # Create DMG with proper layout
    echo "💿 Creating DMG installer..."
    
    # Create temporary DMG directory structure
    DMG_DIR="dmg_temp"
    rm -rf "$DMG_DIR"
    mkdir "$DMG_DIR"
    
    # Copy app to DMG directory
    cp -R ColorRemover.app "$DMG_DIR/"
    
    # Create Applications symlink
    ln -s /Applications "$DMG_DIR/Applications"
    
    # Create README for DMG
    cat > "$DMG_DIR/README.txt" << EOF
ColorRemover v1.0
Professional Image Color Removal Tool

Installation Instructions:
1. Drag ColorRemover.app to the Applications folder
2. Launch from Applications or Launchpad
3. Enjoy removing colors from your images!

Features:
- Smart image color removal
- Bilingual interface (English/Arabic)
- Professional Qt-based interface
- Multiple image format support
- Real-time preview capabilities

Developer: Abdurrhman Saeed
Repository: https://github.com/abdurrhmansaeed/ColorRemover
License: MIT License

System Requirements:
- macOS 10.15 (Catalina) or later
- 512 MB RAM minimum
- 50 MB free disk space
EOF
    
    # Create DMG with custom settings
    DMG_NAME="ColorRemover_v1.0_macOS"
    
    # Method 1: Simple DMG creation
    echo "🔧 Creating simple DMG..."
    hdiutil create -volname "ColorRemover v1.0" \
                   -srcfolder "$DMG_DIR" \
                   -ov \
                   -format UDZO \
                   -imagekey zlib-level=9 \
                   "$DMG_NAME.dmg"
    
    # Method 2: Advanced DMG with create-dmg (if available)
    if command -v create-dmg &> /dev/null; then
        echo "🎨 Creating enhanced DMG with create-dmg..."
        create-dmg \
            --volname "ColorRemover v1.0" \
            --volicon "../icons/app_icon.icns" \
            --window-pos 200 120 \
            --window-size 600 400 \
            --icon-size 100 \
            --icon "ColorRemover.app" 175 190 \
            --hide-extension "ColorRemover.app" \
            --app-drop-link 425 190 \
            --background "../resources/dmg_background.png" \
            "${DMG_NAME}_Enhanced.dmg" \
            "$DMG_DIR/"
    else
        echo "ℹ️  For enhanced DMG, install create-dmg: brew install create-dmg"
    fi
    
    # Clean up
    rm -rf "$DMG_DIR"
    
    # Verify DMG
    if [ -f "$DMG_NAME.dmg" ]; then
        DMG_SIZE=$(du -h "$DMG_NAME.dmg" | cut -f1)
        echo "🎉 DMG created successfully!"
        echo "📁 File: $DMG_NAME.dmg ($DMG_SIZE)"
        echo "📍 Location: $(pwd)/$DMG_NAME.dmg"
        
        # Test mount DMG
        echo "🧪 Testing DMG..."
        hdiutil attach "$DMG_NAME.dmg" -quiet
        if [ $? -eq 0 ]; then
            echo "✅ DMG mounts successfully"
            # Unmount after test
            hdiutil detach "/Volumes/ColorRemover v1.0" -quiet 2>/dev/null || true
        else
            echo "⚠️  DMG created but mount test failed"
        fi
    else
        echo "❌ DMG creation failed!"
        exit 1
    fi
    
    echo ""
    echo "🔗 Repository: https://github.com/abdurrhmansaeed/ColorRemover"
    echo "✅ macOS build complete!"
    
else
    echo "❌ Build failed!"
    exit 1
fi
