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

QMAKE_CXXFLAGS_WARN_ON += -Wall -pedantic


# -----------------------------------------------------------------------

SOURCES += main_server.cpp

# -----------------------------------------------------------------------

unix {
    LIBS += -lboost_thread -lboost_system -lSDL

    HARDWARE_PLATFORM = $$system(uname -m)
    contains( HARDWARE_PLATFORM, x86_64 )::{
        message(Building 64 bit )

        OBJECTS_DIR = tmp/amd64
        DESTDIR = bin/amd64

        # 64-bit Linux
        LIBS += \
            -Llib/amd64 -ltid \
            -Wl,-rpath=$$PWD/lib/amd64 \
            -L../build_amd64/src/tobicore/.libs -ltobicore \
            -Wl,-rpath=$$PWD/../build_amd64/src/tobicore/.libs \
            -L../build_amd64/src/tobiid/.libs -ltobiid \
            -Wl,-rpath=$$PWD/../build_amd64/src/tobiid/.libs

    }else::{
        # 32-bit Linux
        message(Building 32 bit )

        OBJECTS_DIR = tmp/x86
        DESTDIR = bin/x86

        LIBS += -Llib/x86 -ltid \
            -Wl,-rpath=$$PWD/lib/x86 \
            -L../build_x86/src/tobicore/.libs -ltobicore\
            -Wl,-rpath=$$PWD/../build_x86/src/tobicore/.libs \
            -L../build_x86/src/tobiid/.libs -ltobiid \
            -Wl,-rpath=$$PWD/../build_x86/src/tobiid/.libs \
    }

    contains( DEFINES, TIMING_TEST )::{
        LIBS += -lboost_chrono
      }

}

# Note: It is assumed that the boost libraries can be automatically detected by the linker
# through #pragma comment(lib, xxx) declarations in boost.
