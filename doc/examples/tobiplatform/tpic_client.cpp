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
#include <tobicore/TCTime.hpp>

#define ENDLESS

int main(void) {
	ICClass class1("0x301", 0.60f);
	ICClass class2("0x302", 0.40f);
	ICClassifier classifier("cnbi_mi", "CNBI MI Classifier", 
			ICClassifier::ValueProb, 
			ICClassifier::LabelBiosig);
	classifier.classes.Add(&class1);
	classifier.classes.Add(&class2);

	ICMessage message;
	message.classifiers.Add(&classifier);
	message.Dump();
	
	ICSerializerRapid serializer(&message);
		

	TPiC client;
#ifdef ENDLESS
	while(true) {
#endif
		std::cout << "Initializing iC client and trying to plug-in" << std::endl;
		
		if(client.Plug("127.0.0.1", "8000", TPiC::AsClient) != TPiC::Successful) {
#ifdef ENDLESS
			std::cout << "Cannot plug iC client: trying in 5 seconds" << std::endl;
			TCSleep(5000);
			continue;
#else
			std::cout << "Cannot plug iC client" << std::endl;
			return false;
#endif
		}

		int frame = 1;
		while(true) {
			message.SetBlockIdx(++frame);
			if(client.Set(&serializer) != TPiC::Successful)
				break;
			std::cout << "iC message sent: " << frame << std::endl;
			TCSleep(1000);
		}
		std::cout << "iC server is down" << std::endl;
		client.Unplug();
#ifdef ENDLESS
	}
#endif

	return 0;
}
