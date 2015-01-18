/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ICCLASSIFIERS_HPP
#define ICCLASSIFIERS_HPP

#include <string>
#include "ICClassifier.hpp"
#include "ICMapClassifier.hpp"

/*! \brief A set of ICClassifier objects
 *
 * Used by ICMessage to manage multiple ICClassifier objects. 
 * This class is a proxy to an std::map.
 * The ICClassifier objects are indexed with their names.
 *
 * \sa ICClassifier, ICSetClassifierMap, ICMessage
 */
class ICSetClassifier {
	public:
		//! \brief Constructor
		ICSetClassifier(void);
		//! \brief Destructor
		virtual ~ICSetClassifier(void);
		//! \brief Adds an ICClassifier
		virtual ICClassifier* Add(ICClassifier* pclr);
		//! \brief Removes an ICClassifier
		virtual ICClassifier* Remove(std::string nclr);
		//! \brief Removes an ICClassifier
		virtual ICClassifier* Remove(ICClassifier* pclr);
		//! \brief Returns an ICClassifier
		virtual ICClassifier* Get(std::string nclr) const;
		//! \brief Returns an ICClassifier
		virtual ICClassifier* Get(ICClassifier* pclr) const;
		//! \brief Checks if an ICClassifier is present
		virtual bool Has(std::string nclr) const;
		//! \brief Checks if an ICClassifier is present
		virtual bool Has(ICClassifier* pclr) const;
		//! \brief Checks if the ICMapClassifier is empty
		virtual bool Empty(void) const;
		//! \brief Returns the size of the ICMapClassifier
		virtual unsigned int Size(void) const;
		//! \brief Clears the ICMapClassifier
		virtual void Clear(void);
		//! \brief Destroys the ICMapClassifier
		virtual void Destroy(void);
		//! \brief ICMapClassifier iterator
		virtual ICClassifierIter Begin(void);
		//! \brief ICMapClassifier iterator
		virtual ICClassifierIter End(void);
		//! \brief ICMapClassifier constant iterator
		virtual ICClassifierConstIter Begin(void) const;
		//! \brief ICMapClassifier constant iterator
		virtual ICClassifierConstIter End(void) const;
	
	public:
		ICMapClassifier _map;
};

#endif
