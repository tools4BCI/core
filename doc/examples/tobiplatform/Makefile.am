AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = tpclienttcp \
				 tpservertcp

tpclient_SOURCES = tpclient.cpp
tpclient_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpserver_SOURCES = tpserver.cpp
tpserver_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 
