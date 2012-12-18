
TEMPLATE = lib
CONFIG += plugin no_plugin_name_prefix

TARGET = pytid

CYTHON_SOURCES = ../python/pytid.pyx

Qt -= core gui

# get C++ source names from CYTHON_SOURCES
SOURCES = $$CYTHON_SOURCES
SOURCES ~= s/\.pyx\b/.cpp/

# add Cython target for .cpp file generation
cython.target = .cython
cython.commands = cython --cplus $$CYTHON_SOURCES
QMAKE_EXTRA_TARGETS += cython
PRE_TARGETDEPS += .cython

INCLUDEPATH += ../src

GCC_4_7_VAR = $$[USE_GCC_4.7]

contains(GCC_4_7_VAR, 1){
    message( Using GCC 4.7 + optimizations)
    unix:QMAKE_CXX = /usr/bin/g++-4.7
    unix:QMAKE_CXXFLAGS_RELEASE = -O2 -mtune=core2
}

# build and link against system's default python version
QMAKE_CXXFLAGS += $$system(python-config --cflags)

HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    message(Building 64 bit )

    OBJECTS_DIR = tmp/amd64
    DESTDIR = lib/amd64

    LIBS += \
        -L../qmake/lib/amd64 -ltid \
        -Wl,-rpath=$$PWD/../qmake/lib/amd64 \
        -L../build_amd64/src/tobicore/.libs -ltobicore \
        -Wl,-rpath=$$PWD/../build_amd64/src/tobicore/.libs \
        -L../build_amd64/src/tobiid/.libs -ltobiid \
        -Wl,-rpath=$$PWD/../build_amd64/src/tobiid/.libs
}

LIBS += -lboost_thread -lboost_system -lSDL

OTHER_FILES += \
    $$CYTHON_SOURCES
