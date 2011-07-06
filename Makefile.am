EXTRA_DIST = autogen.sh

ACLOCAL_AMFLAGS=-I m4
SUBDIRS = src/tobicore\
		  src/tobiic\
		  src/tobiid\
		  mex\
		  doc


EXTRA_DIST = debian/changelog \
			 debian/compat \
			 debian/control\
			 debian/copyright 
			 debian/rules \
			 debian/libtobicore.install \
			 debian/libtobicore-dev.install

dist-deb: distdir
	am__cwd=`pwd` \
	&& cd $(distdir) \
	&& CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" FFLAGS="$(FFLAGS)" CPPFLAGS="$(CPPFLAGS)" LDFLAGS="$(LD    FLAGS)" dpkg-buildpackage \
	 && cd "$$am__cwd" 
	$(REMOVE_DISTDIR)
