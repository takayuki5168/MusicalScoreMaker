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
           src/event_manager.cpp    \
           src/note_manager.cpp     \
           src/basic_params.cpp    \
           src/mode.cpp

HEADERS  += src/mainwindow.hpp    \
            src/event_manager.hpp    \
            src/note_manager.hpp \
            src/mode.hpp \
            src/math_util.hpp   \
            src/basic_params.hpp

INCLUDEPATH += src              \
               /usr/local/include
