/*
    Copyright (C) 2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobicore library

    The libtobicore library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTTCULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TCLANGUAGE_CPP 
#define TCLANGUAGE_CPP 

#include "TCLanguage.hpp" 
#include "TCTypes.hpp" 
#include <stdio.h>
#include <sstream>
#include <string.h>

std::string TCLanguage::Status(const int component, const int status) {
	std::ostringstream stream;
	stream << "<" << TCSTATUS_ROOTNODE << " " << 
		TCSTATUS_VERSIONNODE << "=\"" << TCSTATUS_VERSION << "\" " << 
		TCSTATUS_COMPONENTNODE << "=\"" << component << "\" " <<
		TCSTATUS_STATUSNODE << "=\"" << status << "\""  << "/>";
	return stream.str();
			
}

bool TCLanguage::CheckVersion(const std::string& message) {
	char version[32];
	sscanf(message.c_str(), "<%*s version=\"%[^'\"']\" %*s/>", version);
	return(strcmp(version, TCSTATUS_VERSION) == 0);
}

bool TCLanguage::IsStatus(const std::string& message, int* component, 
		int* status) {

	int ret = sscanf(message.c_str(), 
			"<tcstatus %*s component=\"%d\" status=\"%d\"/>",
			component, status);
	return(ret == 2);
}

#endif
