#ifndef TPID_CPP 
#define TPID_CPP 

#include "TPiD.hpp" 

TPiD::TPiD(void) {
}

TPiD::~TPiD(void) {
}
		
int TPiD::ConfAsServer(const std::string &ip, const std::string& port) {
	return TPInterface::ErrorNotSupported;
}
		
int TPiD::Set(IDSerializer* serializer) {
	if(TPInterface::_com == NULL)
		return TPInterface::ErrorSocket;
	if(TPInterface::_com->IsConnected() == false)
		return TPInterface::ErrorSocket;
	
	serializer->Serialize(&this->_cache);
	return(TPInterface::_com->Send(TPInterface::_cache) > 0 ? 
			TPInterface::Successful : TPInterface::ErrorSocket);
}

int TPiD::Get(IDSerializer* serializer) {
	if(TPInterface::_com == NULL)
		return TPInterface::ErrorSocket;
	if(TPInterface::_com->IsConnected() == false)
		return TPInterface::ErrorSocket;
	
}

#endif
