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

# Source files - using CI main without resources
SOURCES += \
    main_ci.cpp \
    mainwindow.cpp

# Header files
HEADERS += \
    mainwindow.h

# Platform specific settings
win32 {
    # Fix for Qt 6.x entry point issues
    CONFIG -= embed_manifest_exe
    DEFINES += QT_MAIN_WRAP_ARGC_ARGV
}

# Version information
VERSION = 1.0.0
