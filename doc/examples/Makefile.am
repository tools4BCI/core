AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = tcblock tclanguage \
		 icclass icclassifier icserializerrapid icexample \
		 idexample idserializerrapid idasclient idasserver

tcblock_SOURCES = tcblock.cpp
tcblock_LDADD = $(top_builddir)/src/libtobicore/libtobicore.la
tclanguage_SOURCES = tclanguage.cpp
tclanguage_LDADD = $(top_builddir)/src/libtobicore/libtobicore.la

icclass_SOURCES = icclass.cpp
icclass_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la 
icclassifier_SOURCES = icclassifier.cpp
icclassifier_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la 
icserializerrapid_SOURCES = icserializerrapid.cpp
icserializerrapid_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la 
icexample_SOURCES = icexample.cpp
icexample_LDADD = $(top_builddir)/src/libtobiic/libtobiic.la 

idexample_SOURCES = idexample.cpp
idexample_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la 
idserializerrapid_SOURCES= idserializerrapid.cpp
idserializerrapid_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la 
idasclient_SOURCES = idasclient.cpp
idasclient_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la 
idasserver_SOURCES = idasserver.cpp
idasserver_LDADD = $(top_builddir)/src/libtobiid/libtobiid.la 
