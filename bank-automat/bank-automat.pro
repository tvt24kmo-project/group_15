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
    debitorcredit.cpp \
    environment.cpp \
    historywindow.cpp \
    imagefetcher.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    transactiondata.cpp \
    transfer.cpp \
    withdrawcash.cpp

HEADERS += \
    accountdata.h \
    cardinfo.h \
    debitorcredit.h \
    environment.h \
    historywindow.h \
    imagefetcher.h \
    login.h \
    mainwindow.h \
    transactiondata.h \
    transfer.h \
    withdrawcash.h

FORMS += \
    accountdata.ui \
    cardinfo.ui \
    debitorcredit.ui \
    historywindow.ui \
    login.ui \
    mainwindow.ui \
    transfer.ui \
    withdrawcash.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
