#-------------------------------------------------
#
# Project created by QtCreator 2017-11-25T08:06:52
#
#-------------------------------------------------

QT       += core gui sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LSPE
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    mysql.cpp \
    form_pendaftaran.cpp \
#    simplecrypt.cpp \
    form_peserta.cpp \
    progressdialog.cpp

HEADERS  += mainwindow.h \
    login.h \
    mysql.h \
    utama.h \
    form_pendaftaran.h \
#    simplecrypt.h \
    form_peserta.h \
    progressdialog.h

FORMS    += mainwindow.ui \
    login.ui \
    form_pendaftaran.ui \
    form_peserta.ui \
    progressdialog.ui
