# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libtobiid

noinst_PROGRAMS = idexample \
				  idserializerrapid \
				  asclient \
				  asserver

idexample_SOURCES = idexample.cpp
idserializerrapid_SOURCES= idserializerrapid.cpp
asclient_SOURCES = asclient.cpp
asserver_SOURCES = asserver.cpp

LDADD = $(top_builddir)/src/libtobiid/libtobiid.la
