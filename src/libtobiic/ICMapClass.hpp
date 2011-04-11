/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiic library

    The libtobiic library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ICMAPCLASS_HPP
#define ICMAPCLASS_HPP

#include <map>
#include <string>
#include "ICClass.hpp"

typedef std::map<ICLabel, ICClass*> ICSetClassMap;
typedef ICSetClassMap::iterator ICSetClassIter;
typedef ICSetClassMap::const_iterator ICSetClassConstIter;

#endif
