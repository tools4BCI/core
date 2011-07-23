AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src
lib_LTLIBRARIES = libtobiplatform.la

libtobiplatform_includedir = $(includedir)/tobiplatform

libtobiplatform_include_HEADERS = TPSocket.hpp \
								  TPClient.hpp \
								  TPClientTCP.hpp \
								  TPServer.hpp \
								  TPServerTCP.hpp

libtobiplatform_la_SOURCES = CcMutex.cpp \
							 CcMutex.hpp \
							 CcStreamer.cpp \
							 CcStreamer.hpp \
							 TPClient.cpp \
							 TPClient.hpp \
							 TPClientTCP.cpp \
							 TPClientTCP.hpp \
							 TPServer.cpp \
							 TPServer.hpp \
							 TPServerTCP.cpp \
							 TPServerTCP.hpp \
							 TPSocket.cpp \
							 TPSocket.hpp

libtobiplatform_la_LDFLAGS = $(AM_CFLAGS) -version-info $(CURRENT):$(REVISION):$(AGE) 
libtobiplatform_la_LIBADD = $(top_builddir)/src/tobicore/libtobicore.la \
							$(top_builddir)/src/tobiic/libtobiic.la \
							$(top_builddir)/src/tobiid/libtobiid.la \
							$(PTHREAD_LIBS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tobiplatform.pc
