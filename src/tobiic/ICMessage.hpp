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
#include <tobicore/TCBlock.hpp>
#include <stdint.h>

/*! \brief TOBI iC message
 *
 * \ingroup tobiic
 *
 * An ICMessage is a set of ICClassifier objects stored in an ICSetClassifier
 * map. 
 * Whithin the ICSetClassifier map, each ICClassifier is identified with its
 * name (i.e. "mi_classifier", "errp_classifier"). 
 * ICClassifier objects are added/removed directly via ICSetClassifier.
 *
 * Each ICClassifier is a set of ICClass objects, stored in an ICSetClass
 * map.
 * Whithin the ICSetClass map, each ICClass is identified with its
 * label (i.e. 0x756, 0x562). 
 *
 * Certain methods return a pointer to a classifier or the value of a class
 * belonging to a classifier.
 *
 * An ICMessage can be serialized/deserialized for IPC. As today the only
 * possible serialization is in XML format via the ICSerializerRapid class.
 * Still, users might derive their own serializers from the ICSerializer
 * interface.
 *
 * \sa classifiers, GetClassifier, GetClass, GetValue, SetValue 
 */
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

		/*! \brief Destructor
		 */
		virtual ~ICMessage(void);

		/*! \brief Classifier getter
		 * 
		 * Raises an exception if the classifier was not found
		 *
		 * \arg name ICClassifier name (i.e. "mi_classifier")
		 * \return Reference to ICClassifier
		 */
		virtual ICClassifier* GetClassifier(const std::string& name) const;

		/*! \brief Class getter
		 * 
		 * Raises an exception if the classifier or the class were not found
		 *
		 * \arg name ICClassifier name (i.e. "mi_classifier")
		 * \arg label ICLabel class label (i.e. 0x756)
		 * \return Reference to ICClass
		 */
		virtual ICClass* GetClass(const std::string& name, const ICLabel label) const;

		/*! \brief Class value getter
		 * 
		 * Raises an exception if the classifier or the class were not found
		 *
		 * \arg name ICClassifier name (i.e. "mi_classifier")
		 * \arg label ICLabel class label (i.e. 0x756)
		 * \return Value
		 */
		virtual ICValue GetValue(const std::string& name, const ICLabel label) const;

		/*! \brief Class value setter
		 * 
		 * Raises an exception if the classifier or the class were not found
		 *
		 * \arg name ICClassifier name (i.e. "mi_classifier")
		 * \arg label ICLabel class label (i.e. 0x756)
		 * \arg value ICValue class value (i.e. 0.750)
		 */
		virtual void SetValue(const std::string& name, const ICLabel label, 
				const ICValue value);

		/*! \brief Prints internal data
		 */
		virtual void Dump(void) const;
	public:
		/*! \brief ICClassifier map
		 * 
		 * Set of ICClassifier objects
		 */
		ICSetClassifier classifiers;
};

#endif
