QT += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

# Linux / Ubuntu
unix:!macx {
    CONFIG += link_pkgconfig
    PKGCONFIG += libvlc
}

# Windows
win32 {
    INCLUDEPATH += "C:/Program Files/VideoLAN/VLC/sdk/include"
    LIBS += -L"C:/Program Files/VideoLAN/VLC/sdk/lib" -llibvlc -llibvlccore
}