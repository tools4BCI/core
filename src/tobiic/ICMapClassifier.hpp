/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    It is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ICMAPCLASSIFIER_HPP
#define ICMAPCLASSIFIER_HPP

#include <map>
#include <string>
#include "ICClassifier.hpp"

/*! \brief TOBI iC classifier map
 *
 * \ingroup tobiic
 */
typedef std::map<std::string, ICClassifier*> ICMapClassifier;

/*! \brief TOBI iC classifier map iterator
 *
 * \ingroup tobiic
 */
typedef ICMapClassifier::iterator ICClassifierIter;

/*! \brief TOBI iC classifier map const iterator
 *
 * \ingroup tobiic
 */
typedef ICMapClassifier::const_iterator	ICClassifierConstIter;

#endif
