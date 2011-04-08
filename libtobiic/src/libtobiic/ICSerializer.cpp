/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiic library

    The libtobiid library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ICSerializer.hpp"
#include <string.h>

#ifdef __BORLANDC__
using namespace std;
#endif
		
ICSerializer::ICSerializer(void) {
	this->_message = NULL;
}
		
ICSerializer::ICSerializer(ICMessage* const message) {
	this->_message = message;
}

ICSerializer::~ICSerializer(void) {
	this->_message = NULL;
}
		
ICSerializer* ICSerializer::SetMessage(ICMessage* const message) {
	this->_message = message;
	return this;
}

std::string ICSerializer::SerializeCp(void) {
	std::string cache;
	this->Serialize(&cache);
	return cache;
}

void ICSerializer::DeserializeCp(const std::string& buffer) {
	std::string cache(buffer);
	this->Deserialize(&cache);	
}
		
char* ICSerializer::SerializeCh(char* const buffer, const unsigned int bsize) {
	if(this->_message == NULL)
		return NULL;
	
	std::string cache;
	this->Serialize(&cache);
	if(bsize <= cache.size())
		return NULL;

	memset(buffer, 0, bsize * sizeof(char));
	strcpy(buffer, cache.c_str());
	return buffer;
}

const char* ICSerializer::DeserializeCh(const char* buffer, unsigned int bsize) { 
	if(this->_message == NULL)
		return NULL;

	std::string cache(buffer);
	this->Deserialize(&cache);
	return buffer;
}
