# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libtobiid.la
libtobiid_includedir = $(includedir)/libtobiid
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

libtobiid_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
#libtobiid_la_LIBADD =
