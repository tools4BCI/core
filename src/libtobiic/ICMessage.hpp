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

#ifndef ICMESSAGE_HPP
#define ICMESSAGE_HPP

#include "ICTypes.hpp"
#include "ICClass.hpp"
#include "ICClassifier.hpp"
#include "ICSetClassifier.hpp"
#include <libtobicore/TCBlock.hpp>
#include <stdint.h>

//class DLLEXPORT ICMessage {
class ICMessage : public TCBlock {
	public:
		/*! \brief Constructor
		 */
		ICMessage(void);
		/*! \brief Copy constructor
		 *
		 * Uses serialization/deserialization to copy data structrues
		 *
		 * \arg other Pointer to ICMessage to copy
		 */
		ICMessage(ICMessage* const other);
		virtual ~ICMessage(void);
		/*! \brief Returns a classifier
		 * 
		 * Raises an exception if the classifier was not found
		 *
		 * \arg name ICClassifier name
		 */
		virtual ICClassifier* GetClassifier(const std::string& name) const;
		/*! \brief Returns a class
		 * 
		 * Raises an exception if the classifier or the class were not found
		 *
		 * \arg name ICClassifier name
		 * \arg label ICLabel class label
		 */
		virtual ICClass* GetClass(const std::string& name, const ICLabel label) const;
		/*! \brief Returns class value
		 * 
		 * Raises an exception if the classifier or the class were not found
		 *
		 * \arg name ICClassifier name
		 * \arg label ICLabel class label
		 */
		virtual ICValue GetValue(const std::string& name, const ICLabel label) const;
		/*! \brief Sets class value
		 * 
		 * Raises an exception if the classifier or the class were not found
		 *
		 * \arg name ICClassifier name
		 * \arg label ICLabel class label
		 * \arg value ICValue class value
		 */
		virtual void SetValue(const std::string& name, const ICLabel label, 
				const ICValue value);
		/*! \brief Sets class value
		 *
		 * Dumps internal data structure
		 */
		virtual void Dump(void) const;
	public:
		ICSetClassifier classifiers;
	private:
};

#endif
