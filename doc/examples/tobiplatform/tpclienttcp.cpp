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

int main(void) {
	std::string message;
	TPSocket socket(TPSocket::TCP);
	socket.Open(false);
	socket.Connect("127.0.0.1", "8001");
	socket.Async(true);
	socket.Send("My dear server, here you go.\n");
	
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
