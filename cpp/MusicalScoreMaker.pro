QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

# CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14
# QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override \
#                   -Wno-unused-local-typedef

TARGET = musicalscoremaker
TEMPLATE = app

SOURCES += src/main.cpp             \
           src/mainwindow.cpp    \
           src/event.cpp    \
           src/note.cpp

HEADERS  += src/mainwindow.hpp    \
            src/event.hpp    \
            src/note.hpp

INCLUDEPATH += src              \
               /usr/local/include
