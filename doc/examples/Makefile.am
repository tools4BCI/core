AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = coretcblock \
		 icclass icclassifier icserializerrapid icexample 


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

