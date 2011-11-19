AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = tpclienttcp \
				 tpservertcp \
				 tpicclient \
				 tpicserver \
				 tpidclient 

tpclienttcp_SOURCES = tpclienttcp.cpp
tpclienttcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpservertcp_SOURCES = tpservertcp.cpp
tpservertcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpicclient_SOURCES = tpicclient.cpp
tpicclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la

tpicserver_SOURCES = tpicserver.cpp
tpicserver_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpidclient_SOURCES = tpidclient.cpp
tpidclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la
