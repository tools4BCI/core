interface = tobiic
mexcc = $(interface).cc
mexmw = $(top_srcdir)/mex/mex$(interface)/$(interface).mw
mexld = -l$(interface) -ltobicore
mexpath = ${pkgdatadir}/tobicoremat/mex$(interface)/
if WIN32
mexwin = $(top_srcdir)/mex/mex$(interface)/make.m
verwin = $(RELEASE).$(CURRENT).$(REVISION).$(AGE)
tarwin = mex$(interface)-$(verwin).tar.bz2
endif

cc: $(mexmw)
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
if LINUX
	mex -largeArrayDims $(mexld) $(mexcc)
else
	cp $(mexwin) .
	matlab -nodesktop -wait -r make
	rm make.m
	tar -cjf ../mex$(interface)-$(RELEASE).$(CURRENT).$(REVISION).$(AGE).tar.bz2 *.m *.mex*
endif

clean-local:
	rm -rf $(mexcc)
	rm -rf *.m
	rm -rf *.mex*

install-data-local: mex
	mkdir -p ${mexpath}
	cp *.m ${mexpath}
	cp *.mex* ${mexpath}

uninstall-local:
	rm -rf ${mexpath}
