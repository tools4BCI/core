interface = tobicore
mexcc = $(interface).cc
mexmw = $(top_srcdir)/mex/mex$(interface)/$(interface).mw
mexld = -L$(abs_top_builddir)/src/$(interface)/.libs/ -l$(interface)
mexinc = -I$(top_srcdir)/src/
mexpath = ${pkgdatadir}/tobicoremat/mex$(interface)/
if WIN32
mexwin = $(top_srcdir)/mex/mex$(interface)/make.m
verwin = $(RELEASE).$(CURRENT).$(REVISION).$(AGE)
tarwin = mex$(interface)-$(verwin).tar.bz2
docwin = $(top_srcdir)/doc/examples/mex$(interface)
endif

all: mex

$(interface).cc:
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: $(interface).cc
	mex -largeArrayDims $(mexinc) $(mexcc) $(mexld)
if LINUX
else
	cp $(mexwin) .
	matlab -nodesktop -wait -r make
	rm make.m
	tar -cjf $(top_builddir)/$(tarwin) $(docwin) *.m *.mex*
endif

clean-local:
	rm -rf $(mexcc)
	rm -rf *.m
	rm -rf *.mex*

install-data-local:
	mkdir -p ${mexpath}
	cp *.m ${mexpath}
	cp *.mex* ${mexpath}

uninstall-local:
	rm -rf ${mexpath}
