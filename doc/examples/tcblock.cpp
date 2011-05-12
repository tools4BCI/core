/*
    Copyright (C) 2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobicore library

    The libtobiic library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libtobicore/TCBlock.hpp>
#include <iostream>

using namespace std;


int main(void) {
	TCBlock block0, block1;
	string timestamp, reference;
	
	cout << "1)" << endl;
	cout << block0.absolute.IsSet() << endl;
	cout << block0.relative.IsSet() << endl;
	
	cout << "2)" << endl;
	block0.absolute.Tic();
	cout << block0.absolute.IsSet() << endl;
	cout << block0.relative.IsSet() << endl;

	cout << "3)" << endl;
	block0.relative.Set(&block0.absolute.timestamp);
	cout << block0.absolute.IsSet() << endl;
	cout << block0.relative.IsSet() << endl;
	
	cout << "4)" << endl;
	block0.relative.Unset();
	cout << block0.absolute.IsSet() << endl;
	cout << block0.relative.IsSet() << endl;
	
	cout << "5)" << endl;
	block0.relative.Tic();
	cout << block0.absolute.IsSet() << endl;
	cout << block0.relative.IsSet() << endl;
	
	timestamp.clear();
	reference.clear();
	block0.absolute.Get(&timestamp);
	block0.relative.Get(&reference);
	cout << "t> " << timestamp << endl;
	cout << "r> " << reference << endl;
	
	cout << "t> " << block0.absolute.Toc() << endl;
	cout << "r> " << block0.relative.Toc() << endl;
	
	cout << "6)" << endl;
	cout << block1.absolute.IsSet() << endl;
	cout << block1.relative.IsSet() << endl;
	
	cout << "7)" << endl;
	cout << block1.absolute.Set(timestamp) << endl;
	cout << block1.relative.Set(reference) << endl;
	
	timestamp.clear();
	reference.clear();
	block1.absolute.Get(&timestamp);
	block1.relative.Get(&reference);
	cout << "t> " << timestamp << endl;
	cout << "r> " << reference << endl;

	return 0;
}
