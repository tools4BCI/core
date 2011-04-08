# 09-09-26  Michele Tavella <michele.tavella@epfl.ch>

lib_LTLIBRARIES = libtobiic.la
libtobiic_includedir = $(includedir)/libtobiic
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

libtobiic_la_LDFLAGS = $(AM_CFLAGS) \
						 -version-info $(CURRENT):$(REVISION):$(AGE) \
						 -release $(RELEASE) 
#libtobiic_la_LIBADD =
