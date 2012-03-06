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

#include "ICSetClassifier.hpp"
#include <tobicore/TCException.hpp>

#ifdef __BORLANDC__
using namespace std;
#endif

ICSetClassifier::ICSetClassifier(void) {
}

ICSetClassifier::~ICSetClassifier(void) {
	this->_map.clear();
}

ICClassifier* ICSetClassifier::Add(ICClassifier* pclr) {
    ICClassifierIter it = this->_map.find(pclr->GetName());
	if(it != this->_map.end())
		throw TCException("ICClassifier already present",  
												                       #ifdef _WIN32  
															                         __FUNCSIG__       
												                       #else          
                                                       __PRETTY_FUNCTION__ 
												                       #endif 
		);

	this->_map[pclr->GetName()] = pclr;
	return pclr;
}
	
ICClassifier* ICSetClassifier::Remove(std::string nclr) {
    ICClassifierIter it = this->_map.find(nclr);
	if(it == this->_map.end())
		throw TCException("nclr not found",   
												                       #ifdef _WIN32  
															                         __FUNCSIG__       
												                       #else          
                                                       __PRETTY_FUNCTION__ 
												                       #endif 
    );

	ICClassifier* retval = (*it).second;
	this->_map.erase(it);
	return retval;
}

ICClassifier* ICSetClassifier::Remove(ICClassifier* pclr) {
	if(pclr == NULL)
		throw TCException("pclr is NULL",     
												                       #ifdef _WIN32  
															                         __FUNCSIG__       
												                       #else          
                                                       __PRETTY_FUNCTION__ 
												                       #endif 
    );

	return this->Remove(pclr->GetName());
}

ICClassifier* ICSetClassifier::Get(std::string nclr) const {
    ICClassifierConstIter it = this->_map.find(nclr);
	if(it == this->_map.end())
		throw TCException("nclr not found",    
												                       #ifdef _WIN32  
															                         __FUNCSIG__       
												                       #else          
                                                       __PRETTY_FUNCTION__ 
												                       #endif 
    );

	ICClassifier* retval = (*it).second;
	return retval;
}

ICClassifier* ICSetClassifier::Get(ICClassifier* pclr) const {
	return this->Get(pclr->GetName());
}

bool ICSetClassifier::Has(std::string nclr) const {
    ICClassifierConstIter it = this->_map.find(nclr);
	return(it != this->_map.end());
}

bool ICSetClassifier::Has(ICClassifier* pclr) const {
    ICClassifierConstIter it = this->_map.find(pclr->GetName());
	return(it != this->_map.end());
}

bool ICSetClassifier::Empty(void) const {
	return this->_map.empty();
}
		
unsigned int ICSetClassifier::Size(void) const {
	return this->_map.size();
}

void ICSetClassifier::Clear(void) {
	this->_map.clear();
}

void ICSetClassifier::Destroy(void) {
	ICClassifierIter cit = this->Begin();
	while(cit != this->End()) {
		ICClassifier* cptr = (*cit).second;
		if(cptr != NULL)
			delete cptr;
	}
	this->Clear();
}

ICClassifierIter ICSetClassifier::Begin(void) {
	ICClassifierIter it = this->_map.begin();
	return it;
}

ICClassifierIter ICSetClassifier::End(void) {
	ICClassifierIter it = this->_map.end();
	return it;
}

ICClassifierConstIter ICSetClassifier::Begin(void) const {
	ICClassifierConstIter it = this->_map.begin();
	return it;
}

ICClassifierConstIter ICSetClassifier::End(void) const {
	ICClassifierConstIter it = this->_map.end();
	return it;
}
