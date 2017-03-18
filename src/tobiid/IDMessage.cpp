/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    It is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "IDMessage.hpp"
#include <tobicore/TCException.hpp>
#include <string.h>
#include <stdio.h>
#include <cmath>

#ifdef WIN32
    #ifndef NAN
        static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
        #define NAN (*(const float *) __nan)
    #endif
#endif

#ifdef __BORLANDC__
using namespace std;
#endif

const std::string IDMessage::TxtFamilyUndef("undef");
const std::string IDMessage::TxtFamilyBiosig("biosig");
const std::string IDMessage::TxtFamilyCustom("custom");

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
  this->_family = other.GetFamily();
  this->_description = other.GetDescription();
  this->absolute = other.absolute;
  this->relative = other.relative;
}

IDMessage::IDMessage(IDMessage* const other) {
  //printf("IDMessage -- Pointer Copy constructor ?? \n");
  this->Init();
  this->Copy(other);
}

IDMessage::IDMessage(std::string family, IDevent event) {
  this->Init();
  this->_family = family;
  this->_event = event;
  this->_description = "unset";
}

void IDMessage::Copy(IDMessage* const other) {
  TCBlock::SetBlockIdx(other->GetBlockIdx());
  this->_event = other->GetEvent();
  this->_value = other->GetValue();
  this->_source = other->GetSource();
  this->_family = other->GetFamily();
  this->_description = other->GetDescription();
}

void IDMessage::Init(void) {
  TCBlock::SetBlockIdx(-1);
  this->_family = IDMessage::TxtFamilyUndef;
  this->_event = IDMessage::EventNull;
  this->_value = NAN;
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

std::string IDMessage::GetFamily(void) const {
    IDFvalue fvalue;

//    if(_family == TxtFamilyBiosig)
//      fvalue.assign(IDTYPES_FAMILY_BIOSIG);
//    else if(_family == TxtFamilyCustom)
//        fvalue.assign(IDTYPES_FAMILY_CUSTOM);
//    else if(_family == TxtFamilyUndef)
//      fvalue.assign(IDTYPES_FAMILY_UNDEF);

    //    switch() {
    //      case IDMessage::FamilyBiosig:
    //        fvalue.assign(IDTYPES_FAMILY_BIOSIG);
    //        break;
    //      case IDMessage::FamilyCustom:
    //        fvalue.assign(IDTYPES_FAMILY_CUSTOM);
    //        break;
    //      case IDMessage::FamilyUndef:
    //      default:
    //        fvalue.assign(IDTYPES_FAMILY_UNDEF);
    //        break;
    //    }
  return this->_family;
}

void IDMessage::SetFamily(const std::string& family) {
  //  if(type < IDMessage::FamilyUndef || type > IDMessage::FamilyCustom)
  //    return false;
  this->_family = family;
}

//bool IDMessage::SetFamilyType(const std::string& type)
//{
//  this->_familyType = type;
//  if(type.compare(IDMessage::TxtFamilyUndef) == 0)
//    this->_familyType = IDMessage::FamilyUndef;
//  else if(type.compare(IDMessage::TxtFamilyBiosig) == 0)
//    this->_familyType = IDMessage::FamilyBiosig;
//  else if(type.compare(IDMessage::TxtFamilyCustom) == 0)
//    this->_familyType = IDMessage::FamilyCustom;
//  else
//    return false;
//  return true;
//}

//std::string IDMessage::GetFamilyType(void) const {
//  return this->_familyType;
//}

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
  printf(" + Event family  %s\n", this->GetFamily().c_str());
  printf(" + Event code   %d\n", this->GetEvent());
  printf(" + Event value   %f\n", this->GetValue());
  printf(" + Source    %s\n", this->GetSource().c_str());
}

//std::string IDMessage::FamilyType(IDFvalue family) {
//  if(family.compare(IDTYPES_FAMILY_BIOSIG) == 0)
//    return IDMessage::TxtFamilyBiosig;
//  else if(family.compare(IDTYPES_FAMILY_CUSTOM) == 0)
//      return IDMessage::TxtFamilyCustom;
//  else
//    return IDMessage::TxtFamilyUndef;
//}
