# -----------------------------------------------------------------------
TEMPLATE = app
CONFIG += console \
    thread \
    warn_on \
    exceptions \
    stl
QT -= core \
    gui
DEFINES += TIMING_TEST
TARGET = tid-server

INCLUDEPATH += ../src/libtid \
               ../src
DEPENDPATH += $$INCLUDEPATH
#INCLUDEPATH += extern/include

QMAKE_CXXFLAGS_WARN_ON = -Wall -pedantic


# -----------------------------------------------------------------------

SOURCES += main_server.cpp

# -----------------------------------------------------------------------

unix {
    LIBS += -lboost_thread -lboost_system

    HARDWARE_PLATFORM = $$system(uname -m)
    contains( HARDWARE_PLATFORM, x86_64 )::{
        message(Building 64 bit )

        OBJECTS_DIR = tmp/amd64
        DESTDIR = bin/amd64

        # 64-bit Linux
        LIBS += -Llib/amd64 -ltid \
            -L../build_amd64/src/tobicore/.libs -ltobicore\
            -L../build_amd64/src/tobiid/.libs -ltobiid\

    }else::{
        # 32-bit Linux
        message(Building 32 bit )

        OBJECTS_DIR = tmp/x86
        DESTDIR = bin/x86

        LIBS += -Llib/x86 -ltid \
            -L../build_x86/src/tobicore/.libs -ltobicore\
            -L../build_x86/src/tobiid/.libs -ltobiid
    }

    contains( DEFINES, TIMING_TEST )::{
        LIBS += -lboost_chrono
      }

}

# Note: It is assumed that the boost libraries can be automatically detected by the linker
# through #pragma comment(lib, xxx) declarations in boost.
