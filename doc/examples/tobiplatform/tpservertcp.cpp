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
#include <tobiplatform/TPSocket.hpp>

int main(void) {
	std::string message;
	TPSocket socket(TPSocket::TCP), endpoint(TPSocket::TCP);
	
	if(socket.Open(true) == false) {
		std::cout << "Error: cannot open" << std::endl;
		return 1;
	}

	if(socket.Bind("0.0.0.0", "8000") == false) {
		std::cout << "Error: cannot bind" << std::endl;
		return 1;
	}

	if(socket.Listen() == false) {
		std::cout << "Error: cannot listen" << std::endl;
		return 1;
	} 

	if(socket.Accept(&endpoint) == false) {
		std::cout << "Error: cannot accept" << std::endl;
		return 1;
	}

	std::cout << "Endpoint connected: " << socket.remote.address << std::endl;
	endpoint.Send("My dear client, send me something to die.\n");
	endpoint.Recv(&message);
	std::cout << "Received: " << message << std::endl;
	endpoint.Close();
	socket.Close();
	return 0;
}
