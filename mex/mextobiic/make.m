% 2011-06-04  Michele Tavella <michele.tavella@epfl.ch>
disp('Building MEX interface');
try 
	mex tobiic.cc ../../src/tobiic/.libs/libtobiic.dll.a ../../src/tobicore/.libs/libtobicore.dll.a
	exit;
catch e
	disp('Error: cannot compile');
end
