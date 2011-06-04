% 2011-06-04  Michele Tavella <michele.tavella@epfl.ch>
disp('Building MEX interface');
try 
	mex tobiid.cc c:\minGW\lib\libtobiid.a c:\minGW\lib\libtobicore.a
	exit;
catch e
	disp('Error: cannot compile');
end
