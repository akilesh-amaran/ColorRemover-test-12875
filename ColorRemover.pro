QT += core widgets

CONFIG += c++17

TARGET = ColorRemover
TEMPLATE = app

# Define target directory for debug and release builds
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/debug
}
CONFIG(release, debug|release) {
    DESTDIR = $$PWD/release
}

# Source files
SOURCES += \
    main.cpp \
    mainwindow.cpp

# Header files
HEADERS += \
    mainwindow.h

# Resource files
RESOURCES += \
    resources.qrc

# Application icon for Windows
win32 {
    RC_ICONS = icons/app_icon.ico
    RC_FILE = ColorRemover_resource.rc
}

# Application icon for other platforms  
macx:ICON = icons/app_icon.icns

# Define application version
VERSION = 1.0.0
QMAKE_TARGET_PRODUCT = "Color Remover"
QMAKE_TARGET_DESCRIPTION = "Remove specific colors from images"
QMAKE_TARGET_COMPANY = "ImageEditor"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2025"

# Platform specific settings
win32 {
    CONFIG += windows console  # Add console to fix MinGW linking
    DEFINES += WIN32_LEAN_AND_MEAN
    
    # Fix for MinGW linking issues with Qt 6.x
    mingw {
        QMAKE_LFLAGS += -Wl,--allow-multiple-definition
        LIBS += -ladvapi32
        # Disable Qt6EntryPoint to avoid linking issues
        CONFIG -= qtquickcompiler
        DEFINES += QT_NO_ENTRYPOINT
    }
}

unix:!macx {
    CONFIG += linux
}

macx {
    CONFIG += mac
    ICON = app_icon.icns
}

# Optimization settings
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
    CONFIG += optimize_full
}

# Deployment settings
win32 {
    QTPLUGIN += qwindows qjpeg qgif qico qtiff
}

# Ensure proper linking
LIBS += -luser32 -lgdi32 -lshell32 -lkernel32

# Include paths for headers
INCLUDEPATH += $$PWD

# Define output directory structure
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.rcc
UI_DIR = $$DESTDIR/.ui
