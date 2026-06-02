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
