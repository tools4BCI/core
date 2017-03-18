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

  DEFINES += NDEBUG

#GCC_4_7_VAR = $$[USE_GCC_4.7]

#contains(GCC_4_7_VAR, 1){
#    message( Using GCC 4.7 + optimizations)
#    unix:QMAKE_CXX = /usr/bin/g++-4.7
#    unix:QMAKE_CXXFLAGS_RELEASE = -O2 -mtune=core2
#}

}

#DEFINES += TIMING_TEST
#DEFINES += LPT_TEST

#TEMPLATE = subdirs
SUBDIRS = ../src/libtid/

TARGET = tid
INCLUDEPATH += ../src/libtid \
               ../src

#INCLUDEPATH += E:/local/boost_1_58_0
#LIBS += "-LE:/local/boost_1_58_0/lib32-msvc-11.0"
#LIBS += "-L../msvc10/lib/tobicore/Win32/Debug" -ltobicore

DEPENDPATH += $$INCLUDEPATH
#INCLUDEPATH += extern/include


QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

unix:QMAKE_CXXFLAGS += -pedantic -fPIC -mtune=core2 -std=c++11
unix:QMAKE_CXXFLAGS_WARN_ON += -Wall \
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

