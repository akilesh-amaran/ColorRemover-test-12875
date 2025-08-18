#!/bin/bash
# ColorRemover iOS Build Script  
# Repository: https://github.com/abdurrhmansaeed/ColorRemover

echo "=== ColorRemover iOS Build ==="

# Check if running on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    echo "❌ iOS builds require macOS with Xcode"
    exit 1
fi

# Check if Qt for iOS is installed
if ! command -v qmake &> /dev/null; then
    echo "❌ Qt for iOS not found. Please install Qt with iOS support."
    exit 1
fi

# Check if Xcode is installed
if ! command -v xcodebuild &> /dev/null; then
    echo "❌ Xcode not found. Please install Xcode from App Store."
    exit 1
fi

# Clean previous builds
echo "🧹 Cleaning previous builds..."
rm -rf build_ios/
mkdir -p build_ios/

# Build for iOS
echo "🔨 Building ColorRemover for iOS..."
cd build_ios/

qmake ../ColorRemover.pro \
    -spec macx-ios-clang \
    CONFIG+=release \
    CONFIG+=iphoneos

make -j$(sysctl -n hw.ncpu)

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo "📱 iOS project: build_ios/ColorRemover.xcodeproj"
    echo "ℹ️  Open in Xcode to:"
    echo "   1. Configure signing & capabilities"
    echo "   2. Deploy to device or simulator"
    echo "   3. Archive for App Store submission"
    
    # Open Xcode project
    echo "🚀 Opening Xcode project..."
    open ColorRemover.xcodeproj
    
    echo "🎉 ColorRemover iOS build complete!"
    echo "🔗 Repository: https://github.com/abdurrhmansaeed/ColorRemover"
else
    echo "❌ Build failed!"
    exit 1
fi
