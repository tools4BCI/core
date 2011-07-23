AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = idexample \
				 idserializerrapid \
				 idasclient \
				 idasserver

idexample_SOURCES = idexample.cpp
idexample_LDADD = $(top_builddir)/src/tobiid/libtobiid.la \
				  $(top_builddir)/src/tobicore/libtobicore.la

idserializerrapid_SOURCES= idserializerrapid.cpp
idserializerrapid_LDADD = $(top_builddir)/src/tobiid/libtobiid.la \
						  $(top_builddir)/src/tobicore/libtobicore.la

idasclient_SOURCES = idasclient.cpp
idasclient_LDADD = $(top_builddir)/src/tobiid/libtobiid.la \
				   $(top_builddir)/src/tobicore/libtobicore.la

idasserver_SOURCES = idasserver.cpp
idasserver_LDADD = $(top_builddir)/src/tobiid/libtobiid.la \
				   $(top_builddir)/src/tobicore/libtobicore.la

