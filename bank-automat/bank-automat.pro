QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    accountdata.cpp \
    cardinfo.cpp \
    environment.cpp \
    imagefetcher.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    accountdata.h \
    cardinfo.h \
    environment.h \
    imagefetcher.h \
    login.h \
    mainwindow.h

FORMS += \
    accountdata.ui \
    cardinfo.ui \
    login.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
