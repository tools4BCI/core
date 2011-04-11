# 2010-03-17  Michele Tavella <michele.tavella@epfl.ch>
# Needed just if your environment is not properly configured
#INCPATH=-I/homes/mtavella/Build/include/ 
#LIBPATH=-L/homes/mtavella/Build/lib/ 

# Which library are we linking against?
LDSTUFF=-ltobiic
SOURCE_MW=tobiic.mw
SOURCE_CC=tobiic.cc

# Select platform
PLATFORM=$(shell echo -n `uname -s`)

all: tobiic

tobiic: $(SOURCE_MW)
	@echo "Building and compiling the LibTOBIiC MEX wrapper"
	@echo "Platform is: $(PLATFORM)"
	@echo "Generating wrapper file with MWrap"
	@mwrap -mex tobiic \
		-c tobiic.cc \
		-mb tobiic.mw 
ifeq ($(PLATFORM), Linux)
	@echo "Calling Matlab MEX"
	@mex -largeArrayDims $(INCPATH) $(LIBPATH) $(LDSTUFF) $(SOURCE_CC)
else
	@echo "Please run make.m from Matlab/GNUMex"
endif

clean:
	@rm -vf *.mex*
	@rm -vf *.cc
	@rm -vf tobiic_*.m
	@rm -vf core
