
HEADERS += \
    ../src/libtid/tid_server.h \
    ../src/libtid/tid_exceptions.h \
    ../src/libtid/tid_connection.h \
    ../src/libtid/tcp_connection.h \
    ../src/libtid/input_stream_socket.h \
    ../src/libtid/tid_client_base.h \
    ../src/libtid/tid_client.h \
    ../src/libtid/sdl_tid_client.h \
    ../src/libtid/tcp_server.h \
    ../src/libtid/input_stream.h \
    ../src/libtid/messages/tid_message_types.h \
    ../src/libtid/messages/tid_message_tags_1_0.h \
    ../src/libtid/messages/tid_message_parser_1_0.h \
    ../src/libtid/messages/tid_message_parser.h \
    ../src/libtid/messages/tid_message_builder_1_0.h \
    ../src/libtid/messages/tid_message_builder.h \
    ../src/libtid/messages/tid_message.h

contains( DEFINES, LPT_TEST )::HEADERS += LptTools/LptTools.h

#---------------------------

SOURCES += \
    ../src/libtid/tid_server.cpp \
    ../src/libtid/tid_connection.cpp \
    ../src/libtid/tcp_connection.cpp \
    ../src/libtid/input_stream_socket.cpp \
    ../src/libtid/tid_client_base.cpp \
    ../src/libtid/tid_client.cpp \
    ../src/libtid/sdl_tid_client.cpp \
    ../src/libtid/tcp_server.cpp \
    ../src/libtid/messages/tid_message_parser_1_0.cpp

unix {
  contains( DEFINES, LPT_TEST )::SOURCES += LptTools/LptToolsLinux.cpp
}

win32 {
  contains( DEFINES, LPT_TEST )::SOURCES += LptTools/LptTools.cpp
}


