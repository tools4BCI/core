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
/*! \example 
 */

#include <cstdio>
#include <iostream>

#include <tobiic/ICClass.hpp>
#include <tobiic/ICSetClass.hpp>
#include <tobicore/TCException.hpp>

int main(void) {
	// Let's define 3 classes 
	ICClass class_rh("0x301", 0.10f);
	ICClass class_lh("0x300", 0.20f);
	ICClass class_bf("0x302", 0.70f);
	
	// Lets get all out of class_rh
	std::cout << class_rh.GetValue() << std::endl;
	std::cout << class_rh.GetChValue() << std::endl;
	std::cout << class_rh.GetLabel() << std::endl;
	std::cout << class_rh.GetChLabel() << std::endl;

	/* Now I add all the classes to an ICSetClass container
	 * As you can see, some methods throw exceptions.
	 */
	ICSetClass classes;
	try {
		classes.Add(&class_rh);
		classes.Add(&class_lh);
		classes.Add(&class_bf);
		classes.Add(NULL);
	} catch(TCException e) {
		std::cout << e << std::endl;
	}

	/* We can search the "classes" container for a particular class.
	 * More importantly, classes are indexed by name and can be searched by name
	 * or by reference.
	 */
	std::cout << ">> Has by name" << std::endl;
	std::cout << classes.Has(class_rh.GetLabel()) << std::endl;
	std::cout << classes.Has(class_lh.GetLabel()) << std::endl;
	std::cout << classes.Has(class_bf.GetLabel()) << std::endl;
	std::cout << ">> Has by ptr" << std::endl;
	std::cout << classes.Has(&class_rh) << std::endl;
	std::cout << classes.Has(&class_lh) << std::endl;
	std::cout << classes.Has(&class_bf) << std::endl;
	
	// Getter example to keep you busy
	std::cout << ">> Get by name" << std::endl;
	try {
		std::cout << (classes.Get(class_rh.GetLabel()))->GetLabel() << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}

	// Another getter example
	std::cout << ">> Get by ptr" << std::endl;
	try {
		std::cout << (classes.Get(&class_rh))->GetLabel() << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}
	
	// As you add, you can remove
	std::cout << ">> Remove" << std::endl;
	classes.Remove(&class_rh);
	classes.Remove(class_lh.GetLabel());
	classes.Remove(&class_bf);
	std::cout << classes.Has(&class_rh) << std::endl;
	std::cout << classes.Has(&class_lh) << std::endl;
	std::cout << classes.Has(&class_bf) << std::endl;
	
	// Now that is empty, we should expect an exeption...
	std::cout << ">> Get by name" << std::endl;
	try {
		std::cout << classes.Get(class_rh.GetLabel()) << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}

	// ... here as well
	std::cout << ">> Get by ptr" << std::endl;
	try {
		std::cout << classes.Get(&class_rh) << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}

	return 0;
}
