from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

extensions = [Extension("*", ["*.pyx"])]

setup(
    name='pytid',
    package_data={'' : ['pytid.*']},
    include_package_data=True,
    install_requires=[],
)
