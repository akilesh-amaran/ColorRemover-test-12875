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

# Version information
VERSION = 1.0.0
