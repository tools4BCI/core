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

#include "ICSetClass.hpp"
#include <tobicore/TCException.hpp>
#include <tobicore/TCTools.hpp>

#ifdef __BORLANDC__
using namespace std;
#endif

ICSetClass::ICSetClass(void) {
}

ICSetClass::~ICSetClass(void) {
	this->_map.clear();
}

ICClass* ICSetClass::Add(ICClass* pclass) {
	if(pclass == NULL)
		throw TCException("pclass is NULL", __PRETTY_FUNCTION__);

    ICSetClassIter it = this->_map.find(pclass->GetLabel());
	if(it != this->_map.end())
		throw TCException("ICLabel already present", __PRETTY_FUNCTION__);

	this->_map[pclass->GetLabel()] = pclass;
	return pclass;
}

ICClass* ICSetClass::Remove(ICLabel lclass) {
    ICSetClassIter it = this->_map.find(lclass);
	if(it == this->_map.end())
		throw TCException("ICLabel not found", __PRETTY_FUNCTION__);
	
	ICClass* retval = (*it).second;
	this->_map.erase(it);
	return retval;
}

ICClass* ICSetClass::Remove(unsigned int lclass) {
	return this->Remove(TCTools::itos(lclass));
}

ICClass* ICSetClass::Remove(ICClass* pclass) {
	if(pclass == NULL)
		throw TCException("pclass is NULL", __PRETTY_FUNCTION__);
	
	return this->Remove(pclass->GetLabel());
}

ICClass* ICSetClass::Get(ICLabel lclass) const {
    ICSetClassConstIter it = this->_map.find(lclass);
	if(it == this->_map.end())
		throw TCException("ICLabel not found", __PRETTY_FUNCTION__);

	ICClass* retval = (*it).second;
	return retval;
}

ICClass* ICSetClass::Get(unsigned int lclass) const {
	return this->Get(TCTools::itos(lclass));
}

ICClass* ICSetClass::Get(ICClass* pclass) const {
	return this->Get(pclass->GetLabel());
}

bool ICSetClass::Has(ICLabel name) const {
    ICSetClassConstIter  it = this->_map.find(name);
	return(it != this->_map.end());
}

bool ICSetClass::Has(unsigned int name) const {
	return this->Has(TCTools::itos(name));
}

bool ICSetClass::Has(ICClass* klass) const {
    ICSetClassConstIter it = this->_map.find(klass->GetLabel());
	return(it != this->_map.end());
}

bool ICSetClass::Empty(void) const {
	return this->_map.empty();
}
		
unsigned int ICSetClass::Size(void) const {
	return this->_map.size();
}

void ICSetClass::Clear(void) {
	this->_map.clear();
}

void ICSetClass::Destroy(void) {
	ICSetClassIter kit = this->Begin();
	while(kit != this->End()) {
		ICClass* kptr = (*kit).second;
		if(kptr != NULL)
			delete kptr;
	}
	this->Clear();
}

ICSetClassIter ICSetClass::Begin(void) {
	ICSetClassIter it = this->_map.begin();
	return it;
}

ICSetClassIter ICSetClass::End(void) {
	ICSetClassIter it = this->_map.end();
	return it;
}

ICSetClassConstIter ICSetClass::Begin(void) const {
	ICSetClassConstIter it = this->_map.begin();
	return it;
}

ICSetClassConstIter ICSetClass::End(void) const {
	ICSetClassConstIter it = this->_map.end();
	return it;
}
