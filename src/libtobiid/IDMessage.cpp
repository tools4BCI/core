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

#include "IDMessage.hpp"
#include <libtobicore/TCException.hpp>
#include <string.h>
#include <stdio.h>

#ifdef __BORLANDC__
using namespace std;
#endif

IDMessage::IDMessage(void) {
	this->Init();
}

IDMessage::IDMessage(IDMessage* const other) {
	this->Init();
	this->Copy(other);
}

IDMessage::IDMessage(IDFtype familyType, IDevent event) {
	this->Init();
	this->_familyType = familyType;
	this->_event = event;
	this->_description = "unset";
}

void IDMessage::Copy(IDMessage* const other) {
	TCBlock::SetBlockIdx(other->GetBlockIdx());
	this->_event = other->GetEvent();
	this->_familyType = other->GetFamilyType();
	this->_description = other->GetDescription();
}

void IDMessage::Init(void) {
	TCBlock::SetBlockIdx(-1);
	this->_familyType = IDMessage::FamilyUndef;
	this->_event = IDMessage::EventNull;
}

IDMessage::~IDMessage(void) {
}

std::string IDMessage::GetDescription(void) const {
	return this->_description;
}

void IDMessage::SetDescription(const std::string& description) {
	this->_description = description;
}

IDFvalue IDMessage::GetFamily(void) const {
	IDFvalue fvalue;
	switch(this->_familyType) {
		case IDMessage::FamilyBiosig:
			fvalue.assign(IDTYPES_FAMILY_BIOSIG);
			break;
		case IDMessage::FamilyUndef:
		default:
			fvalue.assign(IDTYPES_FAMILY_UNDEF);
			break;
	}
	return fvalue;
}

void IDMessage::SetFamilyType(const IDFtype type) {
	this->_familyType = type;
}

IDFtype IDMessage::GetFamilyType(void) const {
	return this->_familyType;
}

void IDMessage::SetEvent(const IDevent event) {
	this->_event = event;
}

IDevent IDMessage::GetEvent(void) const {
	return this->_event;
}

void IDMessage::Dump(void) const {
	printf("[IDMessage::Dump] TOBI iD message for frame %d [%s]\n",
			TCBlock::GetBlockIdx(), this->GetDescription().c_str());
	IDFvalue fvalue = this->GetFamily();
	printf(" + Event family  %d/%s\n", this->GetFamilyType(), fvalue.c_str());
	printf(" + Event value   %d\n", this->GetEvent());

}
		
const IDFtype IDMessage::FamilyType(IDFvalue family) {
	if(family.compare(IDTYPES_FAMILY_BIOSIG) == 0)
		return IDMessage::FamilyBiosig;
	else
		return IDMessage::FamilyUndef;
}
