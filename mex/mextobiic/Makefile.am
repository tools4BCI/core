EXTRA_DIST = tobiic.mw.in

interface = tobiic
mexcc = $(interface).cc
mexmw = $(interface).mw
mexld = -l$(interface) -ltobicore

all: mex

cc:
	mwrap -mex $(interface) -c $(mexcc) -mb $(mexmw) 

mex: cc
	mex -largeArrayDims $(mexld) $(mexcc)

clean-local:
	rm -rf *.m
	rm -rf $(mexcc)
	rm -rf *.mex*
