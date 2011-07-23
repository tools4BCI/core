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

#ifndef TPSOCKET_CPP 
#define TPSOCKET_CPP 

#include "TPSocket.hpp" 
#include <stdlib.h>
#include <string.h>

TPHost::TPHost(void) {
	memset(this->address, 0, TPHost::AddressSize);
	this->port = 0;
}

TPSocket::TPSocket(size_t bsize) {
	this->_bsize = bsize;
	this->_mc = 256;
}

TPSocket::~TPSocket(void) {
}

void TPSocket::Init(void) {
	this->_fd = 0;
	this->_mc = 0;
	
	this->_buffer = malloc(this->_bsize);
	memset(this->_buffer, 0, this->_bsize);
	
	memset(&(this->_address), 0, sizeof(this->_address));
	memset(&(this->_results), 0, sizeof(this->_results));
	memset(&(this->_endpoint), 0, sizeof(this->_endpoint));

	this->_bsizemax = 0;
}


#endif
