% 2011-06-04  Michele Tavella <michele.tavella@epfl.ch>
disp('Building MEX interface');
try 
	mex tobicore.cc c:\minGW\lib\libtobicore.dll.a
	exit;
catch e
	disp('Error: cannot compile');
end
