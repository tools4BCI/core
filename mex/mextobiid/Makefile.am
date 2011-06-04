interface = tobiid
mexcc = $(interface).cc
mexmw = $(top_srcdir)/mex/mex$(interface)/$(interface).mw
mexld = -l$(interface) -ltobicore
mexpath = ${pkgdatadir}/tobicoremat/mex$(interface)/
mexwin = $(top_srcdir)/mex/mex$(interface)/make.m

cc: $(mexmw)
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
if LINUX
	mex -largeArrayDims $(mexld) $(mexcc)
else
	cp $(mexwin) .
	matlab -nodesktop -wait -r make.m
	rm make.m
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
