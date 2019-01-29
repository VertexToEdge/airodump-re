TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -pthread


SOURCES += \
        main.cpp \
    bincontainer.cpp \
    radiotap.cpp \
    customanalyzer.cpp \
    ieee80211.cpp \
    ieee80211mac.cpp \
    apinfo.cpp \
    stinfo.cpp


unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libpcap

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libnl-3.0

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libnl-genl-3.0

HEADERS += \
    bincontainer.h \
    pch.h \
    radiotap.h \
    customanalyzer.h \
    radiotap_struct.h \
    ieee80211.h \
    ieee80211mac.h \
    apinfo.h \
    stinfo.h

INCLUDEPATH += "/usr/include/c++/8"
INCLUDEPATH += "/usr/include/x86_64-linux-gnu/c++/8"
INCLUDEPATH += "/usr/include/c++/8/backward"
INCLUDEPATH += "/usr/lib/gcc/x86_64-linux-gnu/8/include"
INCLUDEPATH += "/usr/lib/gcc/x86_64-linux-gnu/8/include"
INCLUDEPATH += "/usr/lib/gcc/x86_64-linux-gnu/8/include"
INCLUDEPATH += "/usr/lib/gcc/x86_64-linux-gnu/8/include-fixed"





unix|win32: LIBS += -lpthread-2.23
