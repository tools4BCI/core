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

#include "TCSerializer.hpp"
#include <string.h>

#ifdef __BORLANDC__
using namespace std;
#endif
		
TCSerializer::TCSerializer(void) {
	this->_message = NULL;
}
		
TCSerializer::TCSerializer(TCBlock* const block) {
	this->_message = block;
}

TCSerializer::~TCSerializer(void) {
	this->_message = NULL;
}
		
TCSerializer* TCSerializer::SetMessage(TCBlock* const block) {
	this->_message = block;
	return this;
}

std::string TCSerializer::SerializeCp(void) {
	std::string cache;
	this->Serialize(&cache);
	return cache;
}

void TCSerializer::DeserializeCp(const std::string& buffer) {
	std::string cache(buffer);
	this->Deserialize(&cache);	
}
		
char* TCSerializer::SerializeCh(char* const buffer, const unsigned int bsize) {
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

const char* TCSerializer::DeserializeCh(const char* buffer, unsigned int bsize) { 
	if(this->_message == NULL)
		return NULL;

	std::string cache(buffer);
	this->Deserialize(&cache);
	return buffer;
}
