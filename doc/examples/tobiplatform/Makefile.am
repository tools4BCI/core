AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = tptcpclient \
				 tptcpserver \
				 tpicclient \
				 tpicserver \
				 tpidclient \
				 tpic

tptcpclient_SOURCES = tptcpclient.cpp
tptcpclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tptcpserver_SOURCES = tptcpserver.cpp
tptcpserver_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpicclient_SOURCES = tpicclient.cpp
tpicclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la

tpicserver_SOURCES = tpicserver.cpp
tpicserver_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpidclient_SOURCES = tpidclient.cpp
tpidclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la
