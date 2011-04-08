# 09-09-23  Michele Tavella <michele.tavella@epfl.ch>
AM_CPPFLAGS = -I$(top_srcdir)/src/libtobiic

noinst_PROGRAMS = icclass \
				  icclassifier \
				  icserializerrapid \
				  icexample

icclass_SOURCES = icclass.cpp
icclassifier_SOURCES = icclassifier.cpp
icserializerrapid_SOURCES = icserializerrapid.cpp
icexample_SOURCES = icexample.cpp

LDADD = $(top_builddir)/src/libtobiic/libtobiic.la
