/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiid library

    The libtobiid library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTIDULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "IDSerializer.hpp"
#include <string.h>

#ifdef __BORLANDC__
using namespace std;
#endif
		
IDSerializer::IDSerializer(void) {
	this->message = NULL;
}
		
IDSerializer::IDSerializer(IDMessage* const message) {
	this->message = message;
}

IDSerializer::~IDSerializer(void) {
	this->message = NULL;
}
		
IDSerializer* IDSerializer::SetMessage(IDMessage* const message) {
	this->message = message;
	return this;
}

std::string IDSerializer::SerializeCp(void) {
	std::string cache;
	this->Serialize(&cache);
	return cache;
}

void IDSerializer::DeserializeCp(const std::string& buffer) {
	std::string cache(buffer);
	this->Deserialize(&cache);	
}
		
char* IDSerializer::SerializeCh(char* const buffer, const unsigned int bsize) {
	if(this->message == NULL)
		return NULL;
	
	std::string cache;
	this->Serialize(&cache);
	if(bsize <= cache.size())
		return NULL;

	memset(buffer, 0, bsize * sizeof(char));
	strcpy(buffer, cache.c_str());
	return buffer;
}

const char* IDSerializer::DeserializeCh(const char* buffer, unsigned int bsize) { 
	if(this->message == NULL)
		return NULL;

	std::string cache(buffer);
	this->Deserialize(&cache);
	return buffer;
}
