AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src
lib_LTLIBRARIES = libtobiid.la

libtobiid_includedir = $(includedir)/tobiid

libtobiid_include_HEADERS = IDTypes.hpp \
							IDMessage.hpp \
							IDSerializer.hpp \
							IDSerializerRapid.hpp \
							IDAsClient.hpp \
							IDAsServer.hpp

libtobiid_la_SOURCES = IDTypes.hpp \
					   IDMessage.cpp \
					   IDMessage.hpp \
					   IDSerializer.cpp \
					   IDSerializer.hpp \
					   IDSerializerRapid.cpp \
					   IDSerializerRapid.hpp \
					   IDAsClient.cpp \
					   IDAsClient.hpp \
					   IDAsServer.hpp \
					   IDAsServer.cpp

libtobiid_la_LDFLAGS = $(AM_CFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE) -no-undefined

libtobiid_la_LIBADD = $(top_builddir)/src/tobicore/libtobicore.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tobiid.pc
