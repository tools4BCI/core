AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = coretcblock \
		 icclass icclassifier icserializerrapid icexample \
		 idexample idserializerrapid idasclient idasserver

coretcblock_SOURCES = coretcblock.cpp
coretcblock_LDADD = $(top_builddir)/src/libtobicore/libtobicore.la

icclass_SOURCES = icclass.cpp
icclass_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la
icclassifier_SOURCES = icclassifier.cpp
icclassifier_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la
icserializerrapid_SOURCES = icserializerrapid.cpp
icserializerrapid_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la
icexample_SOURCES = icexample.cpp
icexample_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la

idexample_SOURCES = idexample.cpp
idexample_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la
idserializerrapid_SOURCES= idserializerrapid.cpp
idserializerrapid_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la
idasclient_SOURCES = idasclient.cpp
idasclient_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la
idasserver_SOURCES = idasserver.cpp
idasserver_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la \
	        $(top_builddir)/src/libtobicore/libtobicore.la
