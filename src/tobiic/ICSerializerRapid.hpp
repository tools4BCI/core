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

#ifndef ICSERIALIZERRAPID_HPP
#define ICSERIALIZERRAPID_HPP

#include "ICMessage.hpp"
#include "ICSerializer.hpp"

/*! \brief TOBI iC XML serializer 
 *
 * \ingroup tobiic
 *
 * Serializes an ICMessage to XML using RapidXML.
 *
 * \sa ICSerializer
 */
class ICSerializerRapid : public ICSerializer {
	public:
		/*! \brief Constructor
		 * \arg message Pointer to an ICMessage instance
		 * \arg indent If true, the XML output will be indented
		 * \arg declaration If true, the XML output will contain an XML
		 * declaration
		 */
		ICSerializerRapid(ICMessage* const message = NULL, 
				const bool indent = false, const bool declaration = false);
		virtual ~ICSerializerRapid(void);
		/*! \brief Serializes to std::string
		 *
		 * \arg buffer Pointer to the std::string that will contain the result
		 * of the serialization
		 * \return Pointer to the buffer instance
		 */
		virtual std::string* Serialize(std::string* buffer);
		/*! \brief Deserializes from std::string
		 *
		 * \arg buffer Pointer to the std::string that contains the result
		 * of a serialization and that will be used for the deserialization
		 * \return A pointer to the buffer instance
		 */
		virtual std::string* Deserialize(std::string* const buffer);

	protected:
		//! \brief Indentation flag
		bool _indent;
		//! \brief Declaration flag
		bool _declaration;
};

#endif
