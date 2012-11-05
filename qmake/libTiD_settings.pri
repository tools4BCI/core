TEMPLATE = lib
VERSION = 1.0
CONFIG += thread \
    warn_on \
    exceptions \
    stl
QT -= core \
    gui


CONFIG( debug, debug|release ) {
    DEFINES += DEBUG
} else {

}

#DEFINES += TIMING_TEST
#DEFINES += LPT_TEST

TARGET = tid

INCLUDEPATH += ../src/libtid \
               ../src

DEPENDPATH += $$INCLUDEPATH
#INCLUDEPATH += extern/include

unix:QMAKE_CXX = /usr/bin/g++-4.7
unix:QMAKE_CXXFLAGS_RELEASE = -O3 -mtune=core2

unix:QMAKE_CXXFLAGS += -pedantic
QMAKE_CXXFLAGS_WARN_ON += -Wall \
    -pedantic

#--------------------------------------------------------------------

HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    message(Building 64 bit )
    OBJECTS_DIR = tmp/amd64
    DESTDIR = lib/amd64
  }else::{
    message(Building 32 bit )
    OBJECTS_DIR = tmp/x86
    DESTDIR = lib/x86
  }

