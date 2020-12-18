QT += widgets
QT += testlib
QT += core gui

CONFIG += c++14 thread

INCLUDEPATH += ../app
SOURCES +=  tst_testmain.cpp \
            ../app/correctionfactor.cpp \
            ../app/filemanager.cpp \
            ../app/mainwindow.cpp \
            ../app/rectifythread.cpp \
            ../app/threadmanager.cpp

RESOURCES += \
    tst_testimage.qrc

HEADERS +=  ../app/correctionfactor.h \
            ../app/filemanager.h \
            ../app/mainwindow.h \
            ../app/rectifythread.h \
            ../app/threadmanager.h

FORMS += ../app/mainwindow.ui

DISTFILES +=
