% 2011-07-09  Michele Tavella <michele.tavella@epfl.ch>
%
% This file is a recursive include script and is meant to be used with
% mtpath:
% - mtpath is distributed with the cnbitoolkit: if you are using the tobicore
% MEX intefaces via cnbitoolkit, mtpath will be used to load tobicoremat
% automatically
% - mtpath can be downloaded from:
%  http://www.mtvl.org/projects/mtpath/
% - still, tobicoremat does not need mtpath to work

[~, basepath] = mtpath_basename(which('tobicoremat_init.m'));

mtpath_include([basepath '/mextobicore']);
mtpath_include([basepath '/mextobiic']);
mtpath_include([basepath '/mextobiid']);
