from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

import platform
bit = platform.architecture()[0]

if bit == '64bit':
  arch = 'amd64'
else:
  arch = 'x86'

if platform.system() == 'Windows':
    # Specify the location of the boost installation dir
    includedirs=['../src',
                 'D:/software/libraries/boost/boost_1_49_0']
    library_dirs=[
      '../msvc10/lib/libtid/Win32/Release',
      '../msvc10/lib/tobiid/Win32/Release',
      '../msvc10/lib/tobicore/Win32/Release',
      'D:/software/libraries/boost/boost_1_49_0/lib_32']
    static_libfiles = [
      '../msvc10/lib/libtid/Win32/Release/libtid.lib',
      '../msvc10/lib/tobiid/Win32/Release/libtid.lib',
      '../msvc10/lib/tobicore/Win32/Release/libtid.lib'
      ]
else:
    includedirs = ['../src']
    lib_dirs=[
      '../qmake/lib/' + arch + '/',
      '../build_' + arch + '/src/tobiid/.libs/',
      '../build_' + arch + '/src/tobicore/.libs/']
    static_libfiles=[
      '../qmake/lib/' + arch + '/libtid.a',
      '../build_' + arch + '/src/tobiid/.libs/libtobiid.a',
      '../build_' + arch + '/src/tobicore/.libs/libtobicore.a']
    


sources = ['pytid.pyx']

setup(name='pytid',
      version='1.0',
      description='Python TiD Client Wrapper',
      url='https://github.com/tools4BCI/core',
      author='Christian Breitwieser',
      author_email='c.breitwieser@tugraz.at',
      license='LGPLv3',
      packages=['pytid'],
      cmdclass = {'build_ext': build_ext},
      ext_modules = [
        Extension(
          name='pytid',
          sources=sources,
          language='c++',
          include_dirs=includedirs,
          library_dirs=lib_dirs,
          libraries=['boost_thread'],
          extra_objects=static_libfiles
        )
    ]
)

