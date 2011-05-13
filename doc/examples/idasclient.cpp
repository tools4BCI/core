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

#include <tobiid/IDAsClient.hpp>

class Client : public IDAsClient {
};

int main(void) {
	IDAsClient fusion;
	fusion.SetBlockIdx(100);

	IDMessage message1(IDMessage::FamilyBiosig, 781);
	IDMessage message2(IDMessage::FamilyBiosig, 782);
	IDMessage message3(IDMessage::FamilyBiosig, 783);
	message1.SetDescription("feedback");
	message2.SetDescription("feedback");
	message3.SetDescription("feedback");
	message1.SetBlockIdx(98);
	message2.SetBlockIdx(99);
	message3.SetBlockIdx(100);

	IDMessage message4(IDMessage::FamilyBiosig, 901);
	IDMessage message5(IDMessage::FamilyBiosig, 902);
	IDMessage message6(IDMessage::FamilyBiosig, 903);
	message4.SetDescription("classifier");
	message5.SetDescription("classifier");
	message6.SetDescription("classifier");
	message4.SetBlockIdx(98);
	message5.SetBlockIdx(99);
	message6.SetBlockIdx(100);

	fusion.Add(message1);
	fusion.Add(message4);
	fusion.Add(message2);
	fusion.Add(message5);
	fusion.Add(message3);
	fusion.Add(message6);
	
	std::cout << "+ Size: " << fusion.Size() << std::endl;
	IDMessage message;
	bool hasmessage = false;
	do {
		hasmessage = fusion.Get(&message, 
				IDMessage::FamilyUndef, 
				IDMessage::EventNull, 
				IDAsClient::BlockAll);
	} while(hasmessage);
	std::cout << "+ Size: " << fusion.Size() << std::endl;
	return 0;
}
