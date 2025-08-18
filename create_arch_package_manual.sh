#!/bin/bash

echo "=== Creating Arch Linux Package Structure ==="

PKG_NAME="colorremover"
PKG_VERSION="1.0.1"
PKG_RELEASE="1"
PKG_ARCH="x86_64"
PKG_FULL_NAME="${PKG_NAME}-${PKG_VERSION}-${PKG_RELEASE}-${PKG_ARCH}"

# Create package directory structure
mkdir -p "${PKG_FULL_NAME}/usr/bin"
mkdir -p "${PKG_FULL_NAME}/usr/share/applications"
mkdir -p "${PKG_FULL_NAME}/usr/share/pixmaps"
mkdir -p "${PKG_FULL_NAME}/usr/share/licenses/${PKG_NAME}"
mkdir -p "${PKG_FULL_NAME}/usr/share/doc/${PKG_NAME}"

# Copy files
cp release/ColorRemover "${PKG_FULL_NAME}/usr/bin/colorremover"
cp debian/usr/share/applications/colorremover.desktop "${PKG_FULL_NAME}/usr/share/applications/"
cp icons/app_icon.png "${PKG_FULL_NAME}/usr/share/pixmaps/colorremover.png"
cp LICENSE "${PKG_FULL_NAME}/usr/share/licenses/${PKG_NAME}/"
cp README.md "${PKG_FULL_NAME}/usr/share/doc/${PKG_NAME}/"

# Make binary executable
chmod 755 "${PKG_FULL_NAME}/usr/bin/colorremover"

# Create .PKGINFO file
cat > "${PKG_FULL_NAME}/.PKGINFO" << PKGINFO
pkgname = ${PKG_NAME}
pkgbase = ${PKG_NAME}
pkgver = ${PKG_VERSION}-${PKG_RELEASE}
pkgdesc = Remove specific colors from images - Qt6 GUI application
url = https://github.com/abdurrhmansaeed/ColorRemover
builddate = $(date +%s)
packager = Abdurrhman Saeed <a1.alsaeed@hotmail.com>
size = $(find "${PKG_FULL_NAME}" -type f -exec du -b {} + | awk '{s+=$1} END {print s}')
arch = ${PKG_ARCH}
license = MIT
depend = qt6-base
depend = gcc-libs
depend = glibc
PKGINFO

# Create .MTREE file (simplified)
cat > "${PKG_FULL_NAME}/.MTREE" << MTREE
#mtree
/set type=file uid=0 gid=0 mode=644
./usr type=dir mode=755
./usr/bin type=dir mode=755
./usr/bin/colorremover mode=755
./usr/share type=dir mode=755
./usr/share/applications type=dir mode=755
./usr/share/applications/colorremover.desktop
./usr/share/pixmaps type=dir mode=755
./usr/share/pixmaps/colorremover.png
./usr/share/licenses type=dir mode=755
./usr/share/licenses/${PKG_NAME} type=dir mode=755
./usr/share/licenses/${PKG_NAME}/LICENSE
./usr/share/doc type=dir mode=755
./usr/share/doc/${PKG_NAME} type=dir mode=755
./usr/share/doc/${PKG_NAME}/README.md
MTREE

# Create package archive
cd "${PKG_FULL_NAME}"
tar -czf "../${PKG_FULL_NAME}.pkg.tar.zst" .
cd ..

echo "✅ Created Arch package: ${PKG_FULL_NAME}.pkg.tar.zst"
ls -lh "${PKG_FULL_NAME}.pkg.tar.zst"

# Create installation instructions
cat > install_arch_package.txt << INSTALL
# Install ColorRemover on Arch Linux

## Method 1: Install directly
sudo pacman -U ${PKG_FULL_NAME}.pkg.tar.zst

## Method 2: Add to local repository
sudo cp ${PKG_FULL_NAME}.pkg.tar.zst /var/cache/pacman/pkg/
sudo pacman -S colorremover

## Usage
colorremover

## Uninstall
sudo pacman -R colorremover
INSTALL

echo "📋 Installation instructions created: install_arch_package.txt"
