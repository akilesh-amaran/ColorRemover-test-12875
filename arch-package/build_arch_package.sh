#!/bin/bash

echo "=== Arch Linux Package Builder ==="
echo "This script creates an Arch Linux package structure"
echo ""

# Check if we're on Arch or have makepkg
if command -v makepkg >/dev/null 2>&1; then
    echo "✅ makepkg found - building package..."
    cd arch-package
    makepkg -si
else
    echo "⚠️  makepkg not found"
    echo ""
    echo "To build on Arch Linux:"
    echo "1. Copy the arch-package/ directory to an Arch system"
    echo "2. cd arch-package"
    echo "3. makepkg -si"
    echo ""
    echo "To build using Docker on Ubuntu:"
    echo "docker run --rm -v \$(pwd):/workspace archlinux/base bash -c \\"
    echo "  'pacman -Sy --noconfirm base-devel git qt6-base qt6-tools && \\"
    echo "   cd /workspace/arch-package && \\"
    echo "   useradd -m builder && \\"
    echo "   chown -R builder:builder /workspace && \\"
    echo "   sudo -u builder makepkg'"
    echo ""
    echo "Creating package info file..."
    
    # Create .PKGINFO file manually
    cat > ../colorremover-1.0.1-1-x86_64.pkg.info << PKGINFO
pkgname = colorremover
pkgbase = colorremover
pkgver = 1.0.1-1
pkgdesc = Remove specific colors from images - Qt6 GUI application
url = https://github.com/abdurrhmansaeed/ColorRemover
builddate = $(date +%s)
packager = Abdurrhman Saeed <a1.alsaeed@hotmail.com>
size = $(du -sb ../release/ColorRemover | cut -f1)
arch = x86_64
license = MIT
depend = qt6-base
depend = qt6-svg
depend = gcc-libs
depend = glibc
PKGINFO

    echo "✅ Package info created: colorremover-1.0.1-1-x86_64.pkg.info"
fi
