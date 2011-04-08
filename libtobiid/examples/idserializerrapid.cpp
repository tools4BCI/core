/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiid library

    The libtobiid library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTIDULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdio>
#include <iostream>

#include "IDMessage.hpp"
#include "IDSerializerRapid.hpp"


int main(void) {
	IDMessage message1(IDMessage::FamilyBiosig, 781);
	message1.SetDescription("module1");
	message1.Dump();
	
	IDMessage message2;
	
	std::string buffer1; IDSerializerRapid serializer1(&message1);
	std::string buffer2; IDSerializerRapid serializer2(&message2);
	message1.SetBlockIdx();
	for(int i = 0; i < 5; i++) {
		message1.IncBlockIdx();
		serializer1.Serialize(&buffer1);
		std::cout << i << ",S) " << buffer1 << std::endl;
		serializer2.Deserialize(&buffer1);
		serializer2.Serialize(&buffer2);
		std::cout << i << ",D) " << buffer1 << std::endl;
	}

	return 0;
}
