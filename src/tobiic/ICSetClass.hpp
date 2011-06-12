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
 * Used by ICClassifier to manage multiple ICClass objects
 * \sa ICClassifier, ICClass, ICMapClass
 */
class ICSetClass {
	public:
		/*! \brief Constructor
		 */
		ICSetClass(void);
		/*! \brief Destructor
		 */
		virtual ~ICSetClass(void);
		/*! \brief Adds an ICClass to ICMapClass
		 *
		 * It raises an exception if a class with the same ICLabel is already
		 * present in ICMapClass or if the argument is NULL.
		 *
		 * \arg pclass pointer to ICClass
		 * \return Pointer to instance
		 */
		virtual ICClass* Add(ICClass* pclass);
		/*! \brief Removes an ICClass from ICMapClass
		 *
		 * It raises an exception if no class with the same ICLabel is present
		 * in ICMapClass or if the argument is NULL.
		 *
		 * \arg lclass class label
		 * \return Pointer to ICClass 
		 */
		virtual ICClass* Remove(ICLabel lclass);
		/*! \brief Removes an ICClass from ICMapClass
		 *
		 * It raises an exception if no class with the same ICLabel is present
		 * in ICMapClass or if the argument is NULL.
		 *
		 * \arg lclass class label
		 * \return Pointer to ICClass 
		 */
		virtual ICClass* Remove(unsigned int lclass);
		/*! \brief Removes an ICClass from ICMapClass
		 *
		 * It raises an exception if no class with the same ICLabel is present
		 * in ICMapClass or if the argument is NULL.
		 *
		 * It raises an exception (TODO) if not found
		 * \return Pointer to ICClass 
		 */
		virtual ICClass* Remove(ICClass* pclass);
		/*! \brief Returns a pointer to ICClass instance
		 *
		 * It raises an exception if no class with the same ICLabel is present.
		 *
		 * \return Pointer to ICClass 
		 */
		virtual ICClass* Get(ICLabel lclass) const;
		/*! \brief Returns a pointer to ICClass instance
		 *
		 * It raises an exception if no class with the same ICLabel is present.
		 *
		 * \return Pointer to ICClass 
		 */
		virtual ICClass* Get(unsigned int lclass) const;
		/*! \brief Returns a pointer to ICClass instance
		 *
		 * It raises an exception if no class with the same ICLabel is present.
		 *
		 * \return Pointer to ICClass 
		 */
		virtual ICClass* Get(ICClass* pclass) const;
		/*! \brief Checks if ICLabel is present
		 *
		 * \return True if present, false otherwise
		 */
		virtual bool Has(ICLabel lclass) const;
		/*! \brief Checks if ICLabel is present
		 *
		 * \return True if present, false otherwise
		 */
		virtual bool Has(unsigned int lclass) const;
		/*! \brief Checks if ICClass is present
		 *
		 * \return True if present, false otherwise
		 */
		virtual bool Has(ICClass* pclass) const;
		/*! \brief Returns the number of classes
		 *
		 * \return The number of classes
		 */
		virtual unsigned int Size(void) const;
		/*! \brief Checks if is empty
		 *
		 * \return True if empty, false otherwise
		 */
		virtual bool Empty(void) const;
		/*! \brief Clear all the stored ICClass pointers 
		 *
		 * This method simply drops the pointers, without calling delete() on
		 * the pointed ICClass instances
		 */
		virtual void Clear(void);
		/*! \brief Destroy and clear all the stored ICClass
		 * 
		 * Before dropping the pointers, calls delete() on the pointed ICClass
		 * instances
		 */
		virtual void Destroy(void);
		/*! \brief Returns an ICSetClassIter
		 */
		virtual ICSetClassIter Begin(void);
		/*! \brief Returns an ICSetClassIter
		 */
		virtual ICSetClassIter End(void);
		/*! \brief Returns an ICSetClassConstIter
		 */
		virtual ICSetClassConstIter Begin(void) const;
		/*! \brief Returns an ICSetClassConstIter
		 */
		virtual ICSetClassConstIter End(void) const;
	protected:
		ICSetClassMap _map;
};

#endif
