# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libtobicore.la

libtobicore_includedir=$(includedir)/libtobicore
libtobicore_include_HEADERS = TCBlock.hpp \
							  TCTimestamp.hpp \
							  TCTools.hpp \
							  TCException.hpp \
							  TCSerializer.hpp

libtobicore_la_SOURCES = TCBlock.cpp \
						 TCBlock.hpp \
						 TCTimestamp.cpp \
						 TCTimestamp.hpp \
						 TCTools.cpp \
						 TCTools.hpp \
						 TCException.cpp \
						 TCException.hpp \
						 TCSerializer.cpp \
						 TCSerializer.hpp

libtobicore_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 


