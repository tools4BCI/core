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
#include <tobiplatform/TPiD.hpp>
#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>
#include <tobicore/TCTime.hpp>

#define ENDLESS true

int main(void) {
	IDMessage message(IDMessage::FamilyBiosig, 1000);
	message.Dump();
	IDSerializerRapid serializer(&message);

	TPiD client;

	while(true) {
		std::cout << "Initializing iD client and trying to plug-in" << std::endl;
		
		if(client.Plug("127.0.0.1", "8126", TPiD::AsClient) != TPiD::Successful) {
#ifdef ENDLESS
			std::cout << "Cannot plug iD client: trying in 5 seconds" << std::endl;
			sleep(5);
			continue;
#else
			std::cout << "Cannot plug iD client" << std::endl;
			return false;
#endif
		}

		int frame = TCBlock::BlockIdxUnset, aframe;
		while(true) {
			if(client.Set(&serializer, frame, &aframe) != TPiD::Successful)
				break;
			std::cout << "iD message sent: " << frame << "/" << aframe << std::endl;
			sleep(1);
		}
		std::cout << "iD server is down" << std::endl;
		client.Unplug();
	}
	
	return 0;
}
