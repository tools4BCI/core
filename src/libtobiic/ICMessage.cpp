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

#include "ICMessage.hpp"
#include "ICSerializerRapid.hpp"
#include <libtobicore/TCException.hpp>
#include <string.h>

#ifdef __BORLANDC__
using namespace std;
#endif

ICMessage::ICMessage(void) {
}
		
ICMessage::ICMessage(ICMessage* const other) {
	std::string buffer;
	ICSerializerRapid serializer(other);
	ICSerializerRapid deserializer(this);
	serializer.Serialize(&buffer);
	deserializer.Deserialize(&buffer);
}

ICMessage::~ICMessage(void) {
	//this->_classifiers.clear();
}

ICClassifier* ICMessage::GetClassifier(const std::string& name) const {
	return this->classifiers.Get(name);
}
		
ICClass* ICMessage::GetClass(const std::string& name, const ICLabel label) const {
	return this->classifiers.Get(name)->classes.Get(label);
}

ICValue ICMessage::GetValue(const std::string& name, const ICLabel label) const {
	return this->classifiers.Get(name)->classes.Get(label)->GetValue();
}
		
void ICMessage::SetValue(const std::string& name, const ICLabel label, const ICValue value) {
	this->classifiers.Get(name)->classes.Get(label)->SetValue(value);
}
	
void ICMessage::Dump(void) const {
	if(this->classifiers.Empty()) 
		throw TCException("Ethernal sunshine of an empty message", __PRETTY_FUNCTION__);
		
	printf("[ICMessage::Dump] TOBI iC message for frame %d\n", GetBlockIdx());
	ICClassifier* cptr = NULL;
	ICClass* kptr = NULL;
	ICClassifierConstIter cit = this->classifiers.Begin();
	ICSetClassConstIter kit;

	while(cit != this->classifiers.End()) {
		cptr = (*cit).second;
		printf(" + Classifier %s [\"%s\", ValueType=%d, LabelType=%d]\n", 
			cptr->GetName().c_str(), cptr->GetDescription().c_str(),
			cptr->GetValueType(), cptr->GetLabelType());
		kit = cptr->classes.Begin();
		while(kit != cptr->classes.End()) {
			kptr = (*kit).second;
			printf("  `--> Class Value=%.6f Label=%s\n", 
					kptr->GetValue(), kptr->GetLabel().c_str());
			++kit;
		}
		++cit;
	}
}
