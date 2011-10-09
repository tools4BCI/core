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

#ifndef TPIC_CPP 
#define TPIC_CPP 

#include "TPiC.hpp" 

TPiC::TPiC(void) {
}

TPiC::~TPiC(void) {
}

int TPiC::Set(ICSerializer* serializer) {
	if(TPInterface::_endpoint != NULL)
		return TPiC::ErrorNotSupported;
	if(TPInterface::_com->IsConnected() == false)
		return TPiC::ErrorSocket;

	serializer->Serialize(&this->_cache);
	return(TPInterface::_com->Send(TPInterface::_cache) > 0 ? 
			TPiC::Successful : TPiC::ErrorSocket);
}

int TPiC::Get(ICSerializer* serializer) {
	if(TPInterface::_endpoint == NULL)
		return TPiC::ErrorNotSupported;
	if(TPInterface::_com->IsConnected() == false)
		return TPiC::ErrorSocket;

	TPInterface::_cache.clear();
	TPInterface::_com->Recv(&this->_cache);
	TPInterface::_stream.Append(TPInterface::_cache);
	
	std::string buffer;
	if(TPInterface::_stream.Extract(&buffer, "<tobiic", "</tobiic>") == true) {
		serializer->Deserialize(&buffer);
		return TPiC::Successful;
	}
	return TPiC::Unsuccessful;
}

#endif
