#!/bin/bash

echo "=== Building Arch Linux Package with Docker ==="

# Create a temporary dockerfile
cat > Dockerfile.arch << DOCKEREOF
FROM archlinux:latest

# Update system and install build tools
RUN pacman -Sy --noconfirm base-devel git qt6-base qt6-tools

# Create a non-root user for building
RUN useradd -m -s /bin/bash builder
RUN echo "builder ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

# Set working directory
WORKDIR /workspace

# Copy source code
COPY . /workspace/

# Change ownership to builder
RUN chown -R builder:builder /workspace

# Switch to builder user
USER builder

# Build the package
WORKDIR /workspace/arch-package
RUN makepkg --noconfirm --skipinteg

# List the created package
RUN ls -la *.pkg.tar.zst
DOCKEREOF

# Create output directory
mkdir -p arch-output

echo "Building Arch package in Docker container..."
sudo docker build -f Dockerfile.arch -t colorremover-arch-builder .

# Extract the package from the container
CONTAINER_ID=$(sudo docker create colorremover-arch-builder)
sudo docker cp ${CONTAINER_ID}:/workspace/arch-package/ arch-output/
sudo docker rm ${CONTAINER_ID}

echo "Arch package should be in arch-output/ directory"
find arch-output/ -name "*.pkg.tar.zst" -exec ls -la {} \;
