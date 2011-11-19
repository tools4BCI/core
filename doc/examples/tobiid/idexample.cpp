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
#include <tobicore/TCException.hpp>

int main(void) {
	// Create 3 ID messages
	IDMessage message1(IDMessage::FamilyBiosig, 781);
	IDMessage message2(IDMessage::FamilyBiosig, 760);
	IDMessage message3(&message2);
	
	// Print internal structure
	message1.Dump();
	message2.Dump();
	message3.Dump();
	return 0;
}
