AM_CPPFLAGS = $(CPPFLAGS) -I$(top_srcdir)/src
lib_LTLIBRARIES = libtobiic.la

libtobiic_includedir = $(includedir)/tobiic

libtobiic_include_HEADERS = ICTypes.hpp \
							ICClass.hpp \
							ICClassifier.hpp \
							ICMessage.hpp \
							ICMapClass.hpp \
							ICMapClassifier.hpp \
							ICSetClass.hpp \
							ICSetClassifier.hpp \
							ICSerializer.hpp \
							ICSerializerRapid.hpp

libtobiic_la_SOURCES = ICTypes.hpp \
					   ICClass.cpp \
					   ICClass.hpp \
					   ICClassifier.cpp \
					   ICClassifier.hpp \
					   ICMessage.cpp \
					   ICMessage.hpp \
					   ICMapClass.hpp \
					   ICMapClassifier.hpp \
					   ICSetClass.cpp \
					   ICSetClass.hpp \
					   ICSetClassifier.cpp \
					   ICSetClassifier.hpp \
					   ICSerializer.cpp \
					   ICSerializer.hpp \
					   ICSerializerRapid.cpp \
					   ICSerializerRapid.hpp

libtobiic_la_LDFLAGS = -version-info $(CURRENT):$(REVISION):$(AGE) -no-undefined

libtobiic_la_LIBADD = $(top_builddir)/src/tobicore/libtobicore.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tobiic.pc
