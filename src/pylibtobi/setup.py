#!/usr/bin/env python

from distutils.core import setup
import os, sys

setup(
        name = 'pylibtobi_interfaces',
        version = '05102011',
        description = 'Python implementation of TOBI interfaces C and D',
        author_email = 'andrew.ramsay@glasgow.ac.uk',
        py_modules = ['pylibtobiic', 'pylibtobiid', 'pylibtobicore'],
    )
