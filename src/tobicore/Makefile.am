lib_LTLIBRARIES = libtobicore.la

libtobicore_includedir=$(includedir)/tobicore
libtobicore_include_HEADERS = TCBlock.hpp \
				TCTimestamp.hpp \
				TCTools.hpp \
				TCException.hpp \
				TCSerializer.hpp \
				TCLanguage.hpp \
				TCTypes.hpp

libtobicore_la_SOURCES = TCBlock.cpp TCBlock.hpp \
			 TCTimestamp.cpp TCTimestamp.hpp \
			 TCTools.cpp  TCTools.hpp \
			 TCException.cpp TCException.hpp \
			 TCSerializer.cpp TCSerializer.hpp \
			 TCLanguage.cpp TCLanguage.hpp \
			 TCTypes.hpp

libtobicore_la_LDFLAGS = -version-info $(CURRENT):$(REVISION):$(AGE)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tobicore.pc
