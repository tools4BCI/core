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

#ifndef ICCLASSES_HPP
#define ICCLASSES_HPP

#include <string>
#include "ICClass.hpp"
#include "ICMapClass.hpp"

/*! \brief A set of ICClass objects
 *
 * \ingroup tobiic
 *
 * Used by ICClassifier to manage multiple ICClass objects. 
 * This class is a proxy to an std::map.
 * The ICClass objects are indexed with their names.
 *
 * \sa ICClassifier, ICClass, ICMapClass
 */
class ICSetClass {
	public:
		//! \brief Constructor
		ICSetClass(void);
		//! \brief Destructor
		virtual ~ICSetClass(void);
		//! \brief Adds an ICClass
		virtual ICClass* Add(ICClass* pclass);
		//! \brief Removes an ICClassifier
		virtual ICClass* Remove(ICLabel lclass);
		//! \brief Removes an ICClassifier
		virtual ICClass* Remove(unsigned int lclass);
		//! \brief Removes an ICClassifier
		virtual ICClass* Remove(ICClass* pclass);
		//! \brief Returns an ICClassifier
		virtual ICClass* Get(ICLabel lclass) const;
		//! \brief Returns an ICClassifier
		virtual ICClass* Get(unsigned int lclass) const;
		//! \brief Returns an ICClassifier
		virtual ICClass* Get(ICClass* pclass) const;
		//! \brief Checks if an ICClassifier is present
		virtual bool Has(ICLabel lclass) const;
		//! \brief Checks if an ICClassifier is present
		virtual bool Has(unsigned int lclass) const;
		//! \brief Checks if an ICClassifier is present
		virtual bool Has(ICClass* pclass) const;
		//! \brief Returns the size of the ICClassMap
		virtual unsigned int Size(void) const;
		//! \brief Checks if the ICMapClass is empty
		virtual bool Empty(void) const;
		//! \brief Clears the ICMapClass
		virtual void Clear(void);
		//! \brief Destroys the ICMapClass
		virtual void Destroy(void);
		//! \brief ICMapClass iterator
		virtual ICSetClassIter Begin(void);
		//! \brief ICMapClass iterator
		virtual ICSetClassIter End(void);
		//! \brief ICMapClass constant iterator
		virtual ICSetClassConstIter Begin(void) const;
		//! \brief ICMapClass constant iterator
		virtual ICSetClassConstIter End(void) const;
	protected:
		ICMapClass _map;
};

#endif
