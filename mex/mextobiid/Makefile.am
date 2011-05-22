EXTRA_DIST = tobiid.mw.in

interface = tobiid
mexcc = $(interface).cc
mexmw = $(interface).mw
mexld = -l$(interface) -ltobicore
mexpath = ${prefix}/opt/cnbitkmat/mex$(interface)/

all: mex

cc: $(mexmw)
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
	mex -largeArrayDims $(mexld) $(mexcc)

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
