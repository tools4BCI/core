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
	this->_socket = NULL;
	this->_endpoint = NULL;
	this->com = NULL;
}

TPiC::~TPiC(void) {
}

int TPiC::Plug(const std::string &ip, const std::string& port, int mode) {
	switch(mode) {
		case TPiC::AsServer:
			return this->ConfAsServer(ip, port);
			break;
		case TPiC::AsClient:
			return this->ConfAsClient(ip, port);
			break;
	}
	return TPiC::ErrorGeneric;
}
		
void TPiC::Unplug(void) {
	if(this->_socket == NULL) 
		return;
	
	this->_socket->Close();
	delete this->_socket;
	this->_socket = NULL;

	this->com = NULL;
	
	if(this->_socket != NULL) {
		delete this->_endpoint;
		this->_endpoint = NULL;
	}
}


int TPiC::ConfAsServer(const std::string &ip, const std::string& port) {
	if(this->_socket != NULL)
		return TPiC::ErrorBound;

	bool status = true;
	this->_socket = new TPSocket(TPSocket::TCP);
	status &= this->_socket->Open(true);
	status &= this->_socket->Bind(ip, port);
	status &= this->_socket->Listen();
	if(status == false) {
		this->Unplug();
		return TPiC::ErrorSocket;
	}

	this->_endpoint = new TPSocket(TPSocket::TCP);
	status = this->_socket->Accept(this->_endpoint);
	if(status == false) {
		this->Unplug();
		return TPiC::ErrorEndpoint;
	}
	
	this->com = this->_endpoint;
	return TPiC::Successful;
}

int TPiC::ConfAsClient(const std::string &ip, const std::string& port) {
	if(this->_socket != NULL)
		return TPiC::ErrorBound;
	
	bool status = true;
	this->_socket = new TPSocket(TPSocket::TCP);
	status &= this->_socket->Open(false);
	status &= this->_socket->Connect(ip, port);
	if(status == false) {
		this->Unplug();
		return TPiC::ErrorSocket;
	}
	
	this->_socket->IsConnected();
	if(this->_socket->IsConnected() == false) {
		this->Unplug();
		return TPiC::ErrorSocket;
	}

	this->com = this->_socket;
	return TPiC::Successful;
}

bool TPiC::IsPlugged(void) {
	if(this->com == NULL)
		return false;
	return this->com->IsConnected();
}

int TPiC::Set(ICSerializer* serializer) {
	if(this->_endpoint != NULL)
		return TPiC::ErrorNotSupported;
	if(this->com->IsConnected() == false)
		return TPiC::ErrorSocket;

	serializer->Serialize(&this->_cache);
	return(this->com->Send(this->_cache) > 0 ? TPiC::Successful : TPiC::ErrorSocket);
}

int TPiC::Get(ICSerializer* serializer) {
	if(this->_endpoint == NULL)
		return TPiC::ErrorNotSupported;
	if(this->com->IsConnected() == false)
		return TPiC::ErrorSocket;

	this->_cache.clear();
	this->com->Recv(&this->_cache);
	this->stream.Append(this->_cache);
	
	std::string buffer;
	if(this->stream.Extract(&buffer, "<tobiic", "</tobiic>") == true) {
		serializer->Deserialize(&buffer);
		return TPiC::Successful;
	}
	return TPiC::Unsuccessful;
}

#endif
