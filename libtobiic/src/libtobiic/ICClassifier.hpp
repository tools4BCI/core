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

class ICClassifier {
	public:
		ICClassifier(const std::string& name, const std::string& desc);
		ICClassifier(const std::string& name, const std::string& desc, 
				ICVtype vtype, ICLtype ltype);
		virtual ~ICClassifier();
		virtual std::string GetName(void) const;
		virtual std::string GetDescription(void) const;
		virtual const char* GetChName(void);
		virtual const char* GetChDescription(void);
		virtual bool SetValueType(ICVtype vtype);
		virtual bool SetLabelType(ICLtype ltype);
		virtual bool SetValueType(std::string vtype);
		virtual bool SetLabelType(std::string ltype);
		virtual ICVtype GetValueType(void) const;
		virtual ICLtype GetLabelType(void) const;

	public:
		static ICVtype ValueType(const std::string& vtype);
		static ICLtype LabelType(const std::string& ltype);
		static ICVtype ValueChType(const char* vtype);
		static ICLtype LabelChType(const char* ltype);

	public:
		//! \brief Undefined class value type
		static const ICVtype ValueUndef = -1;
		//! \brief Probabilities
		static const ICVtype ValueProb  = 0;
		//! \brief Distances
		static const ICVtype ValueDist  = 1;
		//! \brief Class Labels
		static const ICVtype ValueCLbl  = 2;
		//! \brief Regression Coefficients
		static const ICVtype ValueRCoe  = 3;
		
		//! \brief Undefined class label type
		static const ICLtype LabelUndef  = -1;
		//! \brief Biosig Hex labels 
		static const ICLtype LabelBiosig = 0;
		//! \brief Custom labels 
		static const ICLtype LabelCustom = 1;
		//! \brief Class name label
		static const ICLtype LabelClass = 2;

	public:
		ICSetClass classes;

	protected:
		std::string _name; 
		std::string _desc;
		char _chname[ICCLASSIFIER_CHNAME_SIZE];
		char _chdesc[ICCLASSIFIER_CHDESC_SIZE];
		ICVtype _vtype;
		ICLtype _ltype;
};

#endif
