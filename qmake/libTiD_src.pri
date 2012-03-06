
HEADERS += \
    tid_server.h \
    tid_SDL_client.h \
    tid_exceptions.h \
    tid_connection.h \
    tcp_connection.h \
    input_stream_socket.h \
    tid_client.h \
    tcp_server.h \
    input_stream.h \
    messages/tid_message_types.h \
    messages/tid_message_tags_1_0.h \
    messages/tid_message_parser_1_0.h \
    messages/tid_message_parser.h \
    messages/tid_message_builder_1_0.h \
    messages/tid_message_builder.h \
    messages/tid_message.h

contains( DEFINES, LPT_TEST )::HEADERS += LptTools/LptTools.h

#---------------------------

SOURCES += \
    tid_server.cpp \
    tid_connection.cpp \
    tcp_connection.cpp \
    input_stream_socket.cpp \
    tid_client.cpp \
    tcp_server.cpp \
    messages/tid_message_parser_1_0.cpp
    #    tid_SDL_client.cpp

unix {
  contains( DEFINES, LPT_TEST )::SOURCES += LptTools/LptToolsLinux.cpp
}

win32 {
  contains( DEFINES, LPT_TEST )::SOURCES += LptTools/LptTools.cpp
}


