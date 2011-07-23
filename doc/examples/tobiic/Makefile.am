AM_CPPFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = icclass \
				 icclassifier \
				 icserializerrapid \
				 icexample

icclass_SOURCES = icclass.cpp
icclass_LDADD = $(top_builddir)/src/tobiic/libtobiic.la \
				$(top_builddir)/src/tobicore/libtobicore.la

icclassifier_SOURCES = icclassifier.cpp
icclassifier_LDADD = $(top_builddir)/src/tobiic/libtobiic.la \
					 $(top_builddir)/src/tobicore/libtobicore.la

icserializerrapid_SOURCES = icserializerrapid.cpp
icserializerrapid_LDADD = $(top_builddir)/src/tobiic/libtobiic.la  \
						  $(top_builddir)/src/tobicore/libtobicore.la

icexample_SOURCES = icexample.cpp
icexample_LDADD = $(top_builddir)/src/tobiic/libtobiic.la  \
				  $(top_builddir)/src/tobicore/libtobicore.la
