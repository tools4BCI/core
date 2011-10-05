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

#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <tobiplatform/TPiC.hpp>

int main(void) {
	TPiC client;
	std::string buffer;

	if(client.Plug("127.0.0.1", "8001", TPiC::AsServer) != TPiC::Successful) {
	//if(client.Plug("127.0.0.1", "8001", TPiC::AsClient) != TPiC::Successful) {
		std::cout << "Cannot configure iC client" << std::endl;
		return false;
	}


	client.com->Send("Hello world");
	client.com->Recv(&buffer);
	std::cout << "Buffer: " << buffer << std::endl;


	return 0;
}
