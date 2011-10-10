#ifndef TPID_CPP 
#define TPID_CPP 

#include "TPiD.hpp" 
#include <tobicore/TCBlock.hpp>

TPiD::TPiD(void) {
}

TPiD::~TPiD(void) {
}

int TPiD::ConfAsClient(const std::string &ip, const std::string& port) {
	int status = TPInterface::ConfAsClient(ip, port);
	this->_com->Async(true);
	return status;
}
		
int TPiD::ConfAsServer(const std::string &ip, const std::string& port) {
	return TPInterface::ErrorNotSupported;
}

#include <iostream>
int TPiD::Set(IDSerializer* serializer, int bidx, int* abidx) {
	if(TPInterface::_com == NULL)
		return TPInterface::ErrorSocket;
	if(TPInterface::_com->IsConnected() == false)
		return TPInterface::ErrorSocket;
	
	if(abidx != NULL)
		*abidx = TCBlock::BlockIdxUnset;
	
	//TPInterface::_com->Recv(&this->_cache);
	
	serializer->message->SetBlockIdx(bidx);
	serializer->message->absolute.Tic();
	serializer->Serialize(&this->_cache);
	if(TPInterface::_com->Send(TPInterface::_cache) <= 0)
		return TPInterface::ErrorSocket;

	TPInterface::_cache.clear();
	TPInterface::_com->Recv(&this->_cache);
	TPInterface::_stream.Append(TPInterface::_cache);
	
	std::string buffer;
	if(TPInterface::_stream.Extract(&buffer, "<tcstatus", "/>") == false) {
		std::cout << "FUCK ME" << std::endl;
		_stream.Dump();
		sleep(10);
		return TPInterface::ErrorProtocol;
	}
	std::cout << buffer << std::endl;

	int comp, status, fidx;
	if(this->_lang.IsStatus(buffer, &comp, &status, &fidx) == true) {
		if(abidx != NULL) 
			*abidx = fidx;
		return TPInterface::Successful;
	}
	
	return TPInterface::ErrorProtocol;
}

int TPiD::Get(IDSerializer* serializer) {
	if(TPInterface::_com == NULL)
		return TPInterface::ErrorSocket;
	if(TPInterface::_com->IsConnected() == false)
		return TPInterface::ErrorSocket;
	
}

#endif
