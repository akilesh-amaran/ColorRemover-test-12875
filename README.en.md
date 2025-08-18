![App Screenshot](https://github.com/abdurrhmansaeed/ColorRemover/blob/main/images/ColorRemover_en.png?raw=true)
# ColorRemover v1.0

A professional Qt-based application for intelligent color removal from images with bilingual support.

## 🖥️ Downloads

- [windows Full installation](https://github.com/abdurrhmansaeed/ColorRemover/releases/download/v1.0.0/ColorRemover_v1.0_Full_Setup.exe)

- [windows portable](https://github.com/abdurrhmansaeed/ColorRemover/releases/download/v1.0.0/ColorRemover_v1.0_Windows_Portable.zip)

- [Linux Debian-Ubuntu ](https://github.com/abdurrhmansaeed/ColorRemover/releases/download/v1.0.1/colorremover_1.0.0_amd64.deb)

## ✨ Features

- **🖼️ Smart Image Processing**: Load and process various image formats (PNG, JPG, GIF, etc.)
- **🎨 Precise Color Selection**: Advanced color picker with tolerance adjustment
- **👁️ Real-time Preview**: Instant preview of color removal results
- **🔍 Zoom Controls**: Zoom in/out, fit to window, and reset zoom functionality
- **🌍 Bilingual Interface**: Full support for English and Arabic languages
- **💾 Export Options**: Save processed images in multiple formats
- **🖥️ Modern UI**: Clean, intuitive interface with professional design

## 🚀 Quick Start

1. **Open Image**: Click the folder icon or use File → Open Image
2. **Select Color**: Use the color picker tool to choose the color to remove
3. **Adjust Tolerance**: Fine-tune the selection sensitivity
4. **Process**: Click "Remove Color" to apply changes
5. **Save**: Export your edited image using File → Save Edited Image

## 🛠️ Building from Source

### Prerequisites
- Qt 6.9.1 or later
- MinGW compiler (Windows) or equivalent
- Git (for cloning)

### Build Instructions

```bash
# Clone the repository
git clone [repository-url]
cd remove_color_from_img

# Build the project
qmake ColorRemover.pro
mingw32-make

# Run the application
cd release
ColorRemover.exe
```

## 🌐 Language Support

The application automatically detects your system language and supports:
- **English** (en) - Default interface language
- **العربية** (ar) - Full Arabic interface with RTL support

Switch languages anytime via: **Language Menu → Select Language**

## 📋 System Requirements

- **Operating System**: Windows 10 or later
- **Memory**: 512 MB RAM minimum
- **Storage**: 50 MB free space
- **Graphics**: Any modern graphics card with Qt support

## 📁 Project Structure

```
ColorRemover/
├── main.cpp              # Application entry point
├── mainwindow.cpp/.h     # Main application window
├── ColorRemover.pro      # Qt project file
├── resources.qrc         # Qt resource file
├── icons/               # Application icons
├── translations/        # Language files (JSON)
└── ColorRemover_Windows_Portable/  # Portable distribution
```

## 🎯 Version 1.0 Features

- ✅ Complete bilingual interface (English/Arabic)
- ✅ Professional icon and branding
- ✅ Optimized performance and memory usage
- ✅ Comprehensive image format support
- ✅ Advanced color removal algorithms
- ✅ Modern, responsive UI design

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## 👨‍💻 Developer

**Developer**: Abdurrhman Saeed  
**Built with**: GitHub Copilot assistance

---

**ColorRemover v1.0** - Professional Image Color Removal Tool
