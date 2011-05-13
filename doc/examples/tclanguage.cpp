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

#include <libtobicore/TCLanguage.hpp>
#include <iostream>

using namespace std;


int main(void) {

	TCLanguage language;
	std::string status = language.Status(TCLanguage::IC, TCLanguage::Ready);
	cout << status << endl;
	
	cout << "CheckVersion: " << language.CheckVersion(status) << endl;
	
	int c, s;
	cout << "IsStatus: " << language.IsStatus(status, &c, &s) << endl;
	cout << " Component=" << c << endl;
	cout << " Status=" << s << endl;
	
	return 0;
}
