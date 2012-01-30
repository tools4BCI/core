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
#include <tobicore/TCTime.hpp>

/* In this example we connect to a TCP server on localhost:8001 and we 
 * send/receive some messages.
 *
 * You can use this example with tpservertcp.cpp, or you can use your own TCP
 * server.
 */
int main(void) {
	// This is a TCP socket
	TPSocket socket(TPSocket::TCP);
	// And this is a string used as buffer/cache
	std::string message;

	// We open the socket as client...
	socket.Open(false);
	// ... we connect to localhost:80001 ...
	socket.Connect("127.0.0.1", "8001");
	// ... setting the socket to asynchronous operation...
	socket.Async(true);
	// ... and sending a message
	socket.Send("My dear server, here you go.\n");
	
	// Finally, we enter a boring loop
	ssize_t bytes = 0;
	while(true) {
		bytes =  socket.Recv(&message);
		if(bytes > 0)
			std::cout << "Received [" << bytes << "]: " << message << std::endl;
		else {
			TCSleep(1000);
			socket.Send("My dear server, here you go again.\n");
		}
	}
	socket.Close();
	return 0;
}
