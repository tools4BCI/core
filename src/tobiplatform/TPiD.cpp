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

#endif
