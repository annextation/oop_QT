QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = AstronautManager
TEMPLATE = app

SOURCES += \
    main.cpp \
    astronaut.cpp \
    astronaut_doctor.cpp \
    ISS.cpp \
    mainwindow.cpp \
    AstronautEditDialog.cpp

HEADERS += \
    headers.h \
    astronaut.h \
    astronaut_doctor.h \
    ISS.h \
    utils.h \
    mainwindow.h \
    AstronautEditDialog.h

FORMS += \
    mainwindow.ui

# Boost configuration
INCLUDEPATH += "D:\boost_for_qt\boost_1_89_0"
LIBS += -L"D:\boost_for_qt\boost_1_89_0\stage\lib"

CONFIG(debug, debug|release) {
    LIBS += -lboost_serialization-mgw12-mt-d-x64-1_89
} else {
    LIBS += -lboost_serialization-mgw12-mt-x64-1_89
}

win32: LIBS += -lws2_32

# Кодировка
win32: QMAKE_CXXFLAGS += -fexec-charset=UTF-8
