
# -----------------------------------------------------------------------

TEMPLATE += app
CONFIG += console \
    thread \
    warn_on \
    exceptions \
    stl
DESTDIR = tests
TARGET = tid_test
OBJECTS_DIR = tmp/tests

#DEFINES += DEBUG

QT -= core \
      gui

INCLUDEPATH += ../src/libtid \
               ../src \
               ../src/tests
DEPENDPATH += $$INCLUDEPATH

QMAKE_CXXFLAGS_WARN_ON = -Wall -pedantic
unix:QMAKE_CXXFLAGS_RELEASE = -O3

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
    ../src/tests/libtid_send_receive_timing_test.cpp \
    ../src/tests/stream_parsing_tests.cpp

HEADERS += \
    tobiid/IDMessage.hpp\
    tobiid/IDSerializerRapid.hpp\
    tests/statistics.h \
    tests/tid_message_vector_builder.h \
    tests/timed_tid_client.h \
    tests/timed_tid_server.h \
    tests/timed_tid_connection.h \
    tests/timing_test_helpers.h

# -----------------------------------------------------------------------


LIBS += -lboost_thread \
        -lboost_system \
        -lboost_chrono \
        -L../src/tests/UnitTest++

HARDWARE_PLATFORM = $$system(uname -m)
contains( HARDWARE_PLATFORM, x86_64 )::{
    LIBS += ../build_amd64/src/tobiid/.libs/libtobiid.a \
            ../build_amd64/src/tobicore/.libs/libtobicore.a \
            lib/amd64/libtid.a \
            -lUnitTest++_64
  }  else:: {
    LIBS += lib/x86/libtid.a \
            ../build_x86/src/tobicore/.libs/libtobicore.a \
            ../build_x86/src/tobiid/.libs/libtobiid.a \
            -lUnitTest++_64

  }
