AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = tpclienttcp \
				 tpservertcp \
				 tpicclient \
				 tpicserver \
				 tpicclient 

tpclienttcp_SOURCES = tpclienttcp.cpp
tpclienttcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpservertcp_SOURCES = tpservertcp.cpp
tpservertcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpicclient_SOURCES = tpic_client.cpp
tpicclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la

tpicserver_SOURCES = tpic_server.cpp
tpicserver_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpidclient_SOURCES = tpid_client.cpp
tpidclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la
