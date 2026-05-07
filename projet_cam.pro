QT += core gui widgets multimedia multimediawidgets

CONFIG += c++17

# multimedia/multimediawidgets ne sont plus nécessaires :
# on pilote GStreamer via QProcess directement.

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

# Optionnel : active les warnings supplémentaires
# QMAKE_CXXFLAGS += -Wall -Wextra
