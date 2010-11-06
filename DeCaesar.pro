#-------------------------------------------------
#
# Project created by QtCreator 2010-11-04T13:37:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = decaesar
CONFIG   += console release
CONFIG   -= app_bundle debug

TEMPLATE = app

include(config.pri)

DESTDIR = ./bin

INCLUDEPATH += $$BOOST_INCLUDE

LIBS += $$BOOST_LIB

SOURCES += \
    src/main.cxx \
    src/Settings.cxx \
    src/Statistics.cxx \
    src/Analyzer.cxx \
    src/IO.cxx \
    src/AlphabetPermutation.cxx \
    src/MutationStrategy.cxx

HEADERS += \
    src/Settings.h \
    src/Exceptions.h \
    src/Statistics.h \
    src/Analyzer.h \
    src/QStringSerialization.h \
    src/IO.h \
    src/AlphabetPermutation.h \
    src/MutationStrategy.h
