AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = tpclienttcp \
				 tpservertcp \
				 tpic

tpclienttcp_SOURCES = tpclienttcp.cpp
tpclienttcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpservertcp_SOURCES = tpservertcp.cpp
tpservertcp_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 

tpic_SOURCES = tpic.cpp
tpic_LDADD = $(top_builddir)/src/tobiplatform/libtobiplatform.la 
