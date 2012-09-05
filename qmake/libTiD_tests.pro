
# -----------------------------------------------------------------------

TEMPLATE += app
CONFIG += console \
    thread \
    warn_on \
    exceptions \
    stl
TARGET = tid_test
CONFIG( debug, debug|release ) {
    DEFINES += DEBUG
}

DEFINES += \
SKIP_TOBIID_SERIALIZE_TEST \
SKIP_TOBIID_DESERIALIZE_TEST \
SKIP_LIBTID_CLIENT_SEND_TEST \
SKIP_LIBTID_CLIENT_RECV_TEST \
SKIP_LIBTID_LOCALHOST_SEND_RECEIVE_TEST\
#SKIP_LIBTID_SERVER_DISPATCH_TEST \
SKIP_LIBTID_REMOTE_SEND_RECEIVE_TEST\
SKIP_LIBTID_LPT_TEST\
SKIP_STREAM_PARSING_TEST


QT -= core \
      gui

INCLUDEPATH += ../src/libtid \
               ../src \
               ../src/tests
DEPENDPATH += $$INCLUDEPATH

QMAKE_CXXFLAGS_WARN_ON = -Wall -pedantic
unix:QMAKE_CXX = /usr/bin/g++-4.7
unix:QMAKE_CXXFLAGS_RELEASE = -O3 -mtune=core2

#QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
#QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
#LIBS += -lgcov

# -----------------------------------------------------------------------

SOURCES += \
    tests/main.cpp\
    tests/statistics.cpp\
    tests/tobiid_timing_tests.cpp \
    tests/libtid_client_timing_tests.cpp \
    tests/timed_input_stream_socket.h \
    tests/libtid_server_timing_tests.cpp \
    tests/libtid_send_receive_timing_test.cpp \
    tests/timing_test_helpers.cpp \
    tests/libtid_send_LPT_test.cpp \
    tests/stream_parsing_tests.cpp

HEADERS += \
    tests/statistics.h \
    tests/tid_message_vector_builder.h \
    tests/timed_tid_client.h \
    tests/timed_tid_server.h \
    tests/timed_tid_connection.h \
    tests/timing_test_helpers.h \
    tests/LPT_tid_server.h \
    tests/LPT_tid_client.h \
    tests/LPT_tid_connection.h

# -----------------------------------------------------------------------


LIBS += -lboost_thread \
        -lboost_system \
        -lboost_chrono \
        -L../src/tests/UnitTest++

HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    message(Building 64 bit )

    OBJECTS_DIR = tmp/tests/amd64
    DESTDIR = tests/amd64

    LIBS += lib/amd64/libtid.a \
            ../build_amd64/src/tobiid/.libs/libtobiid.a \
            ../build_amd64/src/tobicore/.libs/libtobicore.a \
            -lUnitTest++_64
  }else::{

    OBJECTS_DIR = tmp/tests/x86
    DESTDIR = tests/x86

    message(Building 32 bit )
    LIBS += lib/x86/libtid.a \
            ../build_x86/src/tobiid/.libs/libtobiid.a \
            ../build_x86/src/tobicore/.libs/libtobicore.a \
            -lUnitTest++
  }


