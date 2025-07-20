QT += core widgets multimedia

CONFIG += c++17

TARGET = WorkTimer
TEMPLATE = app

# Source files
SOURCES += \
    src/main.cpp \
    src/worktimer.cpp \
    src/worktimer_methods.cpp \
    src/settings.cpp

# Header files
HEADERS += \
    include/worktimer.h \
    include/settings.h

# Resources
RESOURCES += \
    resources.qrc

# UI files
FORMS += \
    ui/worktimer.ui

# Include directories
INCLUDEPATH += include

# Windows specific settings
win32 {
    RC_ICONS = resources/worktimer_icon.ico
    VERSION = 1.0.0.0
    QMAKE_TARGET_COMPANY = "WorkTimer Team"
    QMAKE_TARGET_PRODUCT = "WorkTimer"
    QMAKE_TARGET_DESCRIPTION = "Pomodoro Timer Application"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2024"
}

# Output directories
DESTDIR = $$PWD/build/bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc
UI_DIR = $$PWD/build/ui

# Copy sounds folder after build
win32 {
    QMAKE_POST_LINK += $$quote(cmd /c \"if not exist \"$$shell_path($$DESTDIR)/sounds\" mkdir \"$$shell_path($$DESTDIR)/sounds\"\")
    QMAKE_POST_LINK += $$quote(cmd /c \"xcopy /E /I /Y \"$$shell_path($$PWD)/sounds\" \"$$shell_path($$DESTDIR)/sounds\"\")
} 