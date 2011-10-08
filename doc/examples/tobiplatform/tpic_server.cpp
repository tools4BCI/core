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

#include <string>
#include <iostream>
#include <unistd.h>
#include <tobiplatform/TPiC.hpp>
#include <tobiic/ICMessage.hpp>
#include <tobiic/ICSerializerRapid.hpp>

int main(void) {
	ICMessage message;
	ICSerializerRapid serializer(&message);

	TPiC server;
	std::string buffer;

	if(server.Plug("127.0.0.1", "8000", TPiC::AsServer) != TPiC::Successful) {
		std::cout << "Cannot plug iC server" << std::endl;
		return false;
	}

	int frame = 1;
	while(true) {
		message.SetBlockIdx(++frame);
		int status = server.Get(&serializer);
		if(status == TPiC::Successful) {
			frame = message.GetBlockIdx();
			//message.Dump();
			std::cout << "iC message received: " << frame << std::endl;
		}
		else if(status == TPiC::Unsuccessful)
			continue;
		else 
			break;

	}
	std::cout << "iC client is down" << std::endl;
	server.Unplug();

	return 0;
}
