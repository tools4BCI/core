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

#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>

int main(void) {
	// I create and ID message
	IDMessage message1(IDMessage::FamilyBiosig, 781);
	message1.SetDescription("module1");
	
	// I create a second message
	IDMessage message2;
	
	// And I print both messages
	message1.Dump();
	message2.Dump();
	
	// I create two buffers and two serializers
	std::string buffer1; IDSerializerRapid serializer1(&message1);
	std::string buffer2; IDSerializerRapid serializer2(&message2);
	
	// I reset the frame number of message1
	message1.SetBlockIdx();
	for(int i = 0; i < 5; i++) {
		// I increment (++) the frame number of message1
		message1.IncBlockIdx();

		// I serialize message1 to buffer1
		serializer1.Serialize(&buffer1);
		std::cout << i << ",S) " << buffer1 << std::endl;
		
		// I deserialize buffer1 into serializer2
		serializer2.Deserialize(&buffer1);

		// And once more, I serialize message2 into buffer2
		serializer2.Serialize(&buffer2);
		std::cout << i << ",D) " << buffer1 << std::endl;
	}

	return 0;
}
