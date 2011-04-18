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

#include "ICClass.hpp"
#include <libtobicore/TCTools.hpp>
#include <libtobicore/TCException.hpp>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#ifdef __BORLANDC__
using namespace std;
#endif
		
ICClass::ICClass(ICLabel label, ICValue value) {
	this->SetValue(value);
	this->SetLabel(label);
}

ICClass::ICClass(unsigned int label, ICValue value) {
	this->SetValue(value);
	this->SetLabel(label);
}

ICClass::~ICClass() {
}

ICValue ICClass::GetValue(void) const {
	return this->_value;
}

ICLabel ICClass::GetLabel(void) const {
	return this->_label;
}

unsigned int ICClass::GetLabelUInt(void) const {
	return atoi(this->_label.c_str());
}

ICClass* ICClass::SetValue(ICValue value) {
	this->_value = value;
	return this;
}

ICClass* ICClass::SetLabel(ICLabel label) {
	if(label.size() > ICCLASS_CHLABEL_SIZE)
		throw TCException("ICCLASS_CHLABEL_SIZE exceeded", __PRETTY_FUNCTION__);
	
	this->_label = label;
	return this;
}

ICClass* ICClass::SetLabel(unsigned int label) {
	std::stringstream stream;
	stream << label;
	
	return this->SetLabel(stream.str());
}

const char* ICClass::GetChLabel(void) {
	memset(this->_chlabel, 0, ICCLASS_CHLABEL_SIZE* sizeof(char));
	strcpy(this->_chlabel, this->_label.c_str());
	return (const char*)(this->_chlabel);
}
		
const char* ICClass::GetChValue(void) {
	memset(this->_chvalue, 0, ICCLASS_CHVALUE_SIZE * sizeof(char));
	TCTools::ftoa(this->_value, this->_chvalue);
	return (const char*)(this->_chvalue);
}

