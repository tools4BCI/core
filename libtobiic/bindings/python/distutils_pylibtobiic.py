#!/usr/bin/env python

from distutils.core import setup, Extension
import sys, os
import datetime
today = datetime.date.today()

setup(name='pylibtobiic',
		version='%02d%02d%04d' % (today.day, today.month, today.year),
		description='libTOBIiC Python wrapper',
		author = 'TOBI project',
		url = 'http://tobi-project.org',
		py_modules = ['pylibtobiic'],
		data_files = [('lib/site-packages', ['libtobiic.dll'])],
		ext_modules = 
		[
			Extension('_pylibtobiic', ['pylibtobiic_wrap.cxx'], include_dirs=['../../src/libtobiic', 'c:/dropbox/tobi/rapidxml', '../mingw_py/include'], library_dirs=['../mingw_py/lib', '.'], libraries=['supc++', 'libtobiic'], define_macros = [('DLLEXPORT', '')]
			)
		]
)

