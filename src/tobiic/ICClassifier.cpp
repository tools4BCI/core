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

#include "ICClassifier.hpp"
#include <tobicore/TCException.hpp>
#include <string.h>

#ifdef __BORLANDC__
using namespace std;
#endif
		
std::string ICClassifier::TxtLabelUndef("LabelUndef");
std::string ICClassifier::TxtLabelBiosig("LabelBiosig");
std::string ICClassifier::TxtLabelCustom("LabelCustom");
std::string ICClassifier::TxtLabelClass("LabelClass");
std::string ICClassifier::TxtValueUndef("ValueUndef");
std::string ICClassifier::TxtValueProb("ValueProb");
std::string ICClassifier::TxtValueDist("ValueDist");
std::string ICClassifier::TxtValueCLbl("ValueCLbl");
std::string ICClassifier::TxtValueRCoe("ValueRCoe");

ICClassifier::ICClassifier(const std::string& name, const std::string& desc) { 
		this->_name = name;
		this->_desc = desc;
		this->_vtype = ICClassifier::ValueUndef;
		this->_ltype = ICClassifier::LabelUndef;
}

ICClassifier::ICClassifier(const std::string& name, const std::string& desc, 
		ICVtype vtype, ICLtype ltype) {
		this->_name = name;
		this->_desc = desc;
		this->_vtype = vtype;
		this->_ltype = ltype;
}

ICClassifier::~ICClassifier() {
}

std::string ICClassifier::GetName(void) const {
	return this->_name;
}

std::string ICClassifier::GetDescription(void) const {
	return this->_desc;
}

const char* ICClassifier::GetChName(void) {
	if(this->_name.size() > ICCLASSIFIER_CHNAME_SIZE)
		throw TCException("ICCLASSIFIER_CHNAME_SIZE exceeded", __PRETTY_FUNCTION__);

	strcpy(this->_chname, this->_name.c_str());
	return this->_chname;
}

const char* ICClassifier::GetChDescription(void) {
	if(this->_desc.size() > ICCLASSIFIER_CHDESC_SIZE)
		throw TCException("ICCLASSIFIER_CHDESC_SIZE exceeded", __PRETTY_FUNCTION__);
	
	strcpy(this->_chdesc, this->_desc.c_str());
	return this->_chdesc;
}

bool ICClassifier::SetValueType(ICVtype vtype) {
	if(vtype < ICClassifier::ValueUndef || vtype > ICClassifier::ValueRCoe)
		return false;
	this->_vtype = vtype;
	return true;
}

bool ICClassifier::SetLabelType(ICLtype ltype) {
	if(ltype < ICClassifier::LabelUndef || ltype > ICClassifier::LabelClass)
		return false;
	this->_ltype = ltype;
	return true;
}

bool ICClassifier::SetValueType(std::string vtype) {
	if(vtype.compare(TxtValueUndef) == 0) 
		this->_vtype = ICClassifier::ValueUndef;
	else if(vtype.compare(TxtValueProb) == 0) 
		this->_vtype = ICClassifier::ValueProb;
	else if(vtype.compare(TxtValueDist) == 0) 
		this->_vtype = ICClassifier::ValueDist;
	else if(vtype.compare(TxtValueCLbl) == 0) 
		this->_vtype = ICClassifier::ValueCLbl;
	else if(vtype.compare(TxtValueRCoe) == 0) 
		this->_vtype = ICClassifier::ValueRCoe;
	else 
		return false;
	return true;
}

bool ICClassifier::SetLabelType(std::string ltype) {
	if(ltype.compare(TxtLabelUndef) == 0) 
		this->_ltype = ICClassifier::LabelUndef;
	else if(ltype.compare(TxtLabelBiosig) == 0) 
		this->_ltype = ICClassifier::LabelBiosig;
	else if(ltype.compare(TxtLabelCustom) == 0) 
		this->_ltype = ICClassifier::LabelCustom;
	else if(ltype.compare(TxtLabelClass) == 0) 
		this->_ltype = ICClassifier::LabelClass;
	else 
		return false;
	return true;
}

		
ICVtype ICClassifier::GetValueType(void) const {
	return this->_vtype;
}

ICLtype ICClassifier::GetLabelType(void) const {
	return this->_ltype;
}
		
ICVtype ICClassifier::ValueType(const std::string& vtype) {
	ICVtype cvtype = ICClassifier::ValueUndef;
	
	if(vtype.compare(ICTYPES_ENTRY_PROB) == 0) 
		cvtype = ICClassifier::ValueProb;
	else if(vtype.compare(ICTYPES_ENTRY_DIST) == 0)
		cvtype = ICClassifier::ValueDist;
	else if(vtype.compare(ICTYPES_ENTRY_CLBL) == 0)
		cvtype = ICClassifier::ValueCLbl;
	else if(vtype.compare(ICTYPES_ENTRY_RCOE) == 0)
		cvtype = ICClassifier::ValueRCoe;
	
	return cvtype;
}

ICLtype ICClassifier::LabelType(const std::string& ltype) {
	ICLtype cltype = ICClassifier::LabelUndef;
		
	if(ltype.compare(ICTYPES_LABEL_CUSTOM) == 0) 
		cltype = ICClassifier::LabelCustom;
	else if(ltype.compare(ICTYPES_LABEL_BIOSIG) == 0) 
		cltype = ICClassifier::LabelBiosig;
	else if(ltype.compare(ICTYPES_LABEL_CLASS) == 0) 
		cltype = ICClassifier::LabelClass;
		
	return cltype;
}

ICVtype ICClassifier::ValueChType(const char* vtype) {
	if(vtype == NULL)
		throw TCException("vtype is NULL", __PRETTY_FUNCTION__);
	ICVtype cvtype = ICClassifier::ValueUndef;
	
	if(strcmp(vtype, ICTYPES_ENTRY_PROB) == 0) 
		cvtype = ICClassifier::ValueProb;
	else if(strcmp(vtype, ICTYPES_ENTRY_DIST) == 0)
		cvtype = ICClassifier::ValueDist;
	else if(strcmp(vtype, ICTYPES_ENTRY_CLBL) == 0)
		cvtype = ICClassifier::ValueCLbl;
	else if(strcmp(vtype, ICTYPES_ENTRY_RCOE) == 0)
		cvtype = ICClassifier::ValueRCoe;
	
	return cvtype;
}

ICLtype ICClassifier::LabelChType(const char* ltype) {
	if(ltype == NULL)
		throw TCException("ltype is NULL", __PRETTY_FUNCTION__);
	ICLtype cltype = ICClassifier::LabelUndef;
		
	if(strcmp(ltype, ICTYPES_LABEL_CUSTOM) == 0) 
		cltype = ICClassifier::LabelCustom;
	else if(strcmp(ltype, ICTYPES_LABEL_BIOSIG) == 0) 
		cltype = ICClassifier::LabelBiosig;
	else if(strcmp(ltype, ICTYPES_LABEL_CLASS) == 0) 
		cltype = ICClassifier::LabelClass;
		
	return cltype;
}
