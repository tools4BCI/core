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

// Endless operation
//#define ENDLESS 


/* In this example we send and receive iD messages.
 *
 * In order to run this example successfully, you need an acquisition server
 * that implements an TOBI iD compliant server. Surely the CnbiToolkit BCI loop
 * implements what you need, and most probably also the acquisition libraries
 * and applications by the TU-Graz BCI team.
 */
int main(void) {
	/* We first need an ID message and an ID concrete serializer
	 * that can convert from and to XML the contents of the ID message
	 */
	IDMessage message(IDMessage::FamilyBiosig, 1000);
	message.Dump();
	IDSerializerRapid serializer(&message);

	/* This is our ID client. It will provide all the functionalities we need
	 * to send an receive ID messages
	 */
	TPiD client;

#ifdef ENDLESS
	while(true) {
#endif
		std::cout << "Initializing iD client and trying to plug-in" << std::endl;
		
		// Connect (Plug...) the client to localhost:8126
		if(client.Plug("127.0.0.1", "8126", TPiD::AsClient) != TPiD::Successful) {
#ifdef ENDLESS
			std::cout << "Cannot plug iD client: trying in 5 seconds" << std::endl;
			TCSleep(5000);
			continue;
#else
			std::cout << "Cannot plug iD client" << std::endl;
			return false;
#endif
		}

		/* We need a frame number to tag outgoing messages (frame)
		 * We will receive the frame number at which the acquisition received
		 * our message on aframe.
		 */
		int frame = TCBlock::BlockIdxUnset, aframe;
		while(true) {
			// Try to send a message, break upon failure
			if(client.Set(&serializer, frame, &aframe) != TPiD::Successful)
				break;
			std::cout << "iD message sent: " << frame << "/" << aframe << std::endl;

			// Keep getting messages until they are available
			while(client.Get(&serializer) != TPInterface::Unsuccessful) {
				std::cout << "iD message received: " << std::endl;
				message.Dump();
			}
			TCSleep(1000.00f);
		}
		// If the iD server is down, disconnect (Unplug...) the client 
		std::cout << "iD server is down" << std::endl;
		client.Unplug();
#ifdef ENDLESS
	}
#endif
	
	return 0;
}
