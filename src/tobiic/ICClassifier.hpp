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

#ifndef ICCLASSIFIER_HPP
#define ICCLASSIFIER_HPP

#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include "ICClass.hpp"
#include "ICSetClass.hpp"
#include "ICTypes.hpp"

#define ICCLASSIFIER_CHNAME_SIZE 	128
#define ICCLASSIFIER_CHDESC_SIZE 	128

/*! \brief TOBI iC classifier
 * 
 * \ingroup tobiic
 * 
 * An ICClassifier is a set of ICClass objects, stored in an ICSetClass
 * map.
 * Whithin the ICSetClass map, each ICClass is identified with its
 * label (i.e. 0x756, 0x562). 
 *
 * Each classifier must be configured for a specific label type (i.e. Biosig)
 * and for a particular value type (i.e. probabilities, regression
 * coefficients).
 *
 * \sa classes
 */
class ICClassifier {
	public:
		/*! \brief Constructor
		 */
		ICClassifier(const std::string& name, const std::string& desc);
		/*! \brief Constructor
		 */
		ICClassifier(const std::string& name, const std::string& desc, 
				ICVtype vtype, ICLtype ltype);
		virtual ~ICClassifier();
		/*! \brief Returns the classifier name
		 */
		virtual std::string GetName(void) const;
		/*! \brief Returns the classifier description
		 */
		virtual std::string GetDescription(void) const;
		/*! \brief Returns the classifier name
		 */
		virtual const char* GetChName(void);
		/*! \brief Returns the classifier description
		 */
		virtual const char* GetChDescription(void);
		/*! \brief Sets the class value type
		 */
		virtual bool SetValueType(ICVtype vtype);
		/*! \brief Sets the class label type
		 *
		 * \arg vtype Label value type
		 * \returns True if successful, false otherwise
		 */
		virtual bool SetLabelType(ICLtype ltype);
		/*! \brief Sets the class value type
		 *
		 * \arg vtype Class value type
		 * \returns True if successful, false otherwise
		 */
		virtual bool SetValueType(std::string vtype);
		/*! \brief Sets the class label type
		 *
		 * \arg vtype Class label type
		 * \returns True if successful, false otherwise
		 */
		virtual bool SetLabelType(std::string ltype);
		/*! \brief Returns the class value type
		 *
		 * \returns vtype Class label type
		 */
		virtual ICVtype GetValueType(void) const;
		/*! \brief Returns the class label type
		 *
		 * \returns Class value type
		 */
		virtual ICLtype GetLabelType(void) const;
	public:
		/*! \brief Converts a value type
		 *
		 * \arg vtype Value type as std::string
		 * \returns Value type
		 */
		static ICVtype ValueType(const std::string& vtype);
		/*! \brief Converts a label type
		 *
		 * \arg ltype Label type as std::string
		 * \returns Label type
		 */
		static ICLtype LabelType(const std::string& ltype);
		/*! \brief Converts a value type
		 *
		 * \arg vtype Value type as char array
		 * \returns Value type
		 */
		static ICVtype ValueChType(const char* vtype);
		/*! \brief Converts a label type
		 * \arg ltype Label type as char array
		 * \returns Label type
		 */
		static ICLtype LabelChType(const char* ltype);

	public:
		//! \brief Undefined class value type
		static const ICVtype ValueUndef = -1;
		//! \brief Probabilities value type
		static const ICVtype ValueProb  = 0;
		//! \brief Distances value type
		static const ICVtype ValueDist  = 1;
		//! \brief Class Labels value type
		static const ICVtype ValueCLbl  = 2;
		//! \brief Regression coefficients value type
		static const ICVtype ValueRCoe  = 3;
		
		//! \brief Undefined class label type
		static const ICLtype LabelUndef  = -1;
		//! \brief Biosig label type
		static const ICLtype LabelBiosig = 0;
		//! \brief Custom label type
		static const ICLtype LabelCustom = 1;
		//! \brief Class name label type
		static const ICLtype LabelClass = 2;
		
		//! \brief Undefined class value type as string
		static std::string TxtValueUndef;
		//! \brief Probabilities value type as string
		static std::string TxtValueProb;
		//! \brief Distances value type as string
		static std::string TxtValueDist;
		//! \brief Class Labels value type as string
		static std::string TxtValueCLbl;
		//! \brief Regression coefficients value type as string
		static std::string TxtValueRCoe;
		//! \brief Undefined class label type as string
		static std::string TxtLabelUndef;
		//! \brief Biosig label type as string
		static std::string TxtLabelBiosig;
		//! \brief Custom label type as string
		static std::string TxtLabelCustom;
		//! \brief Class name label type as string
		static std::string TxtLabelClass;
	
	public:
		/*! \brief ICClass map
		 * 
		 * Set of ICClass objects
		 */
		ICSetClass classes;
	protected:
		//! \brief Classifier name
		std::string _name; 
		//! \brief Classifier description
		std::string _desc;
		//! \brief Classifier name for in-situ serialization
		char _chname[ICCLASSIFIER_CHNAME_SIZE];
		//! \brief Classifier description for in-situ serialization
		char _chdesc[ICCLASSIFIER_CHDESC_SIZE];
		//! \brief Value type
		ICVtype _vtype;
		//! \brief Label type
		ICLtype _ltype;
};

#endif
