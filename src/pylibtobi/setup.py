#!/usr/bin/env python

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.

# It is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public License
# along with this file.  If not, see <http://www.gnu.org/licenses/>.



from distutils.core import setup
import os, sys

setup(
        name = 'pylibtobi_interfaces',
        version = '05102011',
        description = 'Python implementation of TOBI interfaces C and D',
        author_email = 'andrew.ramsay@glasgow.ac.uk',
        py_modules = ['pylibtobiic', 'pylibtobiid', 'pylibtobicore'],
    )
