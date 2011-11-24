% 2011-06-04  Michele Tavella <michele.tavella@epfl.ch>
disp('Building MEX interface');
try 
	mex tobicore.cc ../../src/tobicore/.libs/libtobicore.dll.a -I../../../src/
	exit;
catch e
	disp('Error: cannot compile');
end
