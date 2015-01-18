from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

sources = ['pytid.pyx']

setup(
	cmdclass = {'build_ext': build_ext},
	ext_modules = [Extension(
		name='pytid', 
		sources=sources, 
		language='c++',
		include_dirs=[
			'../src', 
			'D:/software/libraries/boost/boost_1_49_0_speed_mt'],
		library_dirs=[
			'../msvc10/lib/libtid/Win32/Release',
			'../msvc10/lib/tobiid/Win32/Release',
			'../msvc10/lib/tobicore/Win32/Release',
			'D:/software/libraries/boost/boost_1_49_0_speed_mt/lib_32'],
		libraries=['libtid','tobiid','tobicore'],
		extra_compile_args=['/EHsc']
	)]
	#name = 'pytid',
	#ext_modules = cythonize('pytid.pyx'),
	#include_dirs=['../src', 'D:/software/libraries/boost/boost_1_49_0_speed_mt'],
	#extra_compile_args=['blabla']
)