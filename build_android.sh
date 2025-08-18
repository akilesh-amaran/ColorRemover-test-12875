#!/bin/bash
# ColorRemover Android Build Script
# Repository: https://github.com/abdurrhmansaeed/ColorRemover

echo "=== ColorRemover Android Build ==="

# Check requirements
if [ -z "$ANDROID_SDK_ROOT" ]; then
    echo "❌ ANDROID_SDK_ROOT not set. Please install Android SDK."
    echo "   Download from: https://developer.android.com/studio"
    exit 1
fi

if [ -z "$JAVA_HOME" ]; then
    echo "❌ JAVA_HOME not set. Please install JDK 11+."
    exit 1
fi

if ! command -v qmake &> /dev/null; then
    echo "❌ Qt for Android not found. Please install Qt with Android support."
    exit 1
fi

# Clean previous builds
echo "🧹 Cleaning previous builds..."
rm -rf build_android/
mkdir -p build_android/

# Build for Android
echo "🔨 Building ColorRemover for Android..."
cd build_android/

qmake ../ColorRemover.pro \
    -spec android-clang \
    CONFIG+=release \
    ANDROID_TARGET_ARCH=armeabi-v7a

make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    
    # Create APK
    echo "📦 Creating APK..."
    make apk
    
    if [ -f "android-build/build/outputs/apk/debug/android-build-debug.apk" ]; then
        echo "🎉 ColorRemover.apk created successfully!"
        echo "📱 APK location: build_android/android-build/build/outputs/apk/"
        echo "🔗 Repository: https://github.com/abdurrhmansaeed/ColorRemover"
        
        # Copy APK to project root for easy access
        cp android-build/build/outputs/apk/debug/android-build-debug.apk ../ColorRemover-Android.apk
        echo "📋 APK copied to: ColorRemover-Android.apk"
    else
        echo "❌ APK creation failed!"
        exit 1
    fi
else
    echo "❌ Build failed!"
    exit 1
fi
