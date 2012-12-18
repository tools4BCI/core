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
#include <tobicore/TCException.hpp>
#include <string.h>
#include <stdio.h>

#ifdef __BORLANDC__
using namespace std;
#endif

std::string IDMessage::TxtFamilyUndef("FamilyUndef");
std::string IDMessage::TxtFamilyBiosig("FamilyBiosig");
std::string IDMessage::TxtFamilyCustom("FamilyCustom");

IDMessage::IDMessage(void) {
  this->Init();
}

IDMessage::~IDMessage(void) {
}

IDMessage::IDMessage(const IDMessage& other) {
  //printf("IDMessage -- Own Copy constructor ?? \n");
  this->Init();
  //this->Copy(&other);

  TCBlock::SetBlockIdx(other.GetBlockIdx());
  this->_event = other.GetEvent();
  this->_value = other.GetValue();
  this->_source = other.GetSource();
  this->_familyType = other.GetFamilyType();
  this->_description = other.GetDescription();
}

IDMessage::IDMessage(IDMessage* const other) {
  //printf("IDMessage -- Pointer Copy constructor ?? \n");
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
  this->_value = other->GetValue();
  this->_source = other->GetSource();
  this->_familyType = other->GetFamilyType();
  this->_description = other->GetDescription();
}

void IDMessage::Init(void) {
  TCBlock::SetBlockIdx(-1);
  this->_familyType = IDMessage::FamilyUndef;
  this->_event = IDMessage::EventNull;
  this->_value = 0;
}

std::string IDMessage::GetDescription(void) const {
  return this->_description;
}

void IDMessage::SetDescription(const std::string& description) {
  this->_description = description;
}

std::string IDMessage::GetSource(void) const {
  return this->_source;
}

void IDMessage::SetSource(const std::string& source) {
  this->_source = source;
}

IDFvalue IDMessage::GetFamily(void) const {
  IDFvalue fvalue;
  switch(this->_familyType) {
    case IDMessage::FamilyBiosig:
      fvalue.assign(IDTYPES_FAMILY_BIOSIG);
      break;
    case IDMessage::FamilyCustom:
      fvalue.assign(IDTYPES_FAMILY_CUSTOM);
      break;
    case IDMessage::FamilyUndef:
    default:
      fvalue.assign(IDTYPES_FAMILY_UNDEF);
      break;
  }
  return fvalue;
}

bool IDMessage::SetFamilyType(const IDFtype type) {
  if(type < IDMessage::FamilyUndef || type > IDMessage::FamilyCustom)
    return false;
  this->_familyType = type;
  return true;
}

bool IDMessage::SetFamilyType(const std::string& type) {
  if(type.compare(IDMessage::TxtFamilyUndef) == 0)
    this->_familyType = IDMessage::FamilyUndef;
  else if(type.compare(IDMessage::TxtFamilyBiosig) == 0)
    this->_familyType = IDMessage::FamilyBiosig;
  else if(type.compare(IDMessage::TxtFamilyCustom) == 0)
    this->_familyType = IDMessage::FamilyCustom;
  else
    return false;
  return true;
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

void IDMessage::SetValue(const IDvalue value ) {
  this->_value = value;
}

IDvalue IDMessage::GetValue(void) const {
  return this->_value;
}

void IDMessage::Dump(void) const {
  printf("[IDMessage::Dump] TOBI iD message for frame %d [%s]\n",
      TCBlock::GetBlockIdx(), this->GetDescription().c_str());
  IDFvalue fvalue = this->GetFamily();
  printf(" + Event family  %d/%s\n", this->GetFamilyType(), fvalue.c_str());
  printf(" + Event code   %d\n", this->GetEvent());
  printf(" + Event value   %d\n", this->GetValue());

}

const IDFtype IDMessage::FamilyType(IDFvalue family) {
  if(family.compare(IDTYPES_FAMILY_BIOSIG) == 0)
    return IDMessage::FamilyBiosig;
  else
    return IDMessage::FamilyUndef;
}
