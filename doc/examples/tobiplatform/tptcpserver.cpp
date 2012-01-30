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

/* In this example we bind a TCP socket and we communicate with a TCP client.
 *
 * You can use this example with tpclienttcp.cpp, or you can use your own TCP
 * client.
 */
int main(void) {
	// I create a TCP socket and a TCP endpoint
	TPSocket socket(TPSocket::TCP), endpoint(TPSocket::TCP);
	
	// This string will work as buffer/cache
	std::string message;
	
	// Here I open the socket, as server
	if(socket.Open(true) == false) {
		std::cout << "Error: cannot open" << std::endl;
		return 1;
	}
	
	// Here I bind the socket to all local addresses and TCP port 8001
	if(socket.Bind("0.0.0.0", "8001") == false) {
		std::cout << "Error: cannot bind" << std::endl;
		return 1;
	}

	// Put the socket in listening mode
	if(socket.Listen() == false) {
		std::cout << "Error: cannot listen" << std::endl;
		return 1;
	} 

	// Wait for an endpoint to connect
	if(socket.Accept(&endpoint) == false) {
		std::cout << "Error: cannot accept" << std::endl;
		return 1;
	}

	// If Accept() returns true, an endpoint is connected...
	std::cout << "Endpoint connected: " << socket.remote.address << std::endl;

	// .. and we can send a message to it...
	endpoint.Send("My dear client, send me something to die.\n");

	// ... or even receive a message from it.
	endpoint.Recv(&message);
	std::cout << "Received: " << message << std::endl;

	// Now we go down closing all sockets and endpoints
	endpoint.Close();
	socket.Close();
	return 0;
}
