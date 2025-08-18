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

# Resources
RESOURCES += \
    resources.qrc

# Version information
VERSION = 1.0.0
QMAKE_TARGET_PRODUCT = "Color Remover"
QMAKE_TARGET_DESCRIPTION = "Remove specific colors from images"
QMAKE_TARGET_COMPANY = "ImageEditor"
QMAKE_TARGET_COPYRIGHT = "Copyright (c) 2025"

# Platform specific settings
win32 {
    RC_FILE = ColorRemover_resource.rc
}

unix:!macx {
    # Linux specific settings
    target.path = /usr/local/bin
    INSTALLS += target
}

macx {
    ICON = icons/app_icon.icns
}

# Optimization settings
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}
