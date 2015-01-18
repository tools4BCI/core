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

#GCC_4_7_VAR = $$[USE_GCC_4.7]

#contains(GCC_4_7_VAR, 1){
#    message( Using GCC 4.7 + optimizations)
#    unix:QMAKE_CXX = /usr/bin/g++-4.7
#    unix:QMAKE_CXXFLAGS_RELEASE = -O2 -mtune=core2
#}

}

#DEFINES += TIMING_TEST
#DEFINES += LPT_TEST

TARGET = tid

INCLUDEPATH += ../src/libtid \
               ../src

DEPENDPATH += $$INCLUDEPATH
#INCLUDEPATH += extern/include




unix:QMAKE_CXXFLAGS += -pedantic -fPIC
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

