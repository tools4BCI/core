AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = tpclienttcp \
				 tpservertcp

tpclienttcp_SOURCES = tpclienttcp.cpp
tpclienttcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpservertcp_SOURCES = tpservertcp.cpp
tpservertcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 
