/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiic library

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

#include <cstdio>
#include <iostream>

#include <libtobiic/ICClass.hpp>
#include <libtobiic/ICSetClass.hpp>
#include <libtobicore/TCException.hpp>

int main(void) {
	ICClass class_rh("0x301", 0.10f);
	ICClass class_lh("0x300", 0.20f);
	ICClass class_bf("0x302", 0.70f);
	
	std::cout << class_rh.GetValue() << std::endl;
	std::cout << class_rh.GetChValue() << std::endl;
	std::cout << class_rh.GetLabel() << std::endl;
	std::cout << class_rh.GetChLabel() << std::endl;

	ICSetClass classes;
	try {
		classes.Add(&class_rh);
		classes.Add(&class_lh);
		classes.Add(&class_bf);
		classes.Add(NULL);
	} catch(TCException e) {
		std::cout << e << std::endl;
	}


	std::cout << ">> Has by name" << std::endl;
	std::cout << classes.Has(class_rh.GetLabel()) << std::endl;
	std::cout << classes.Has(class_lh.GetLabel()) << std::endl;
	std::cout << classes.Has(class_bf.GetLabel()) << std::endl;
	std::cout << ">> Has by ptr" << std::endl;
	std::cout << classes.Has(&class_rh) << std::endl;
	std::cout << classes.Has(&class_lh) << std::endl;
	std::cout << classes.Has(&class_bf) << std::endl;
	
	std::cout << ">> Get by name" << std::endl;
	try {
		std::cout << (classes.Get(class_rh.GetLabel()))->GetLabel() << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}
	std::cout << ">> Get by ptr" << std::endl;
	try {
		std::cout << (classes.Get(&class_rh))->GetLabel() << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}
	
	std::cout << ">> Remove" << std::endl;
	classes.Remove(&class_rh);
	classes.Remove(class_lh.GetLabel());
	classes.Remove(&class_bf);
	std::cout << classes.Has(&class_rh) << std::endl;
	std::cout << classes.Has(&class_lh) << std::endl;
	std::cout << classes.Has(&class_bf) << std::endl;
	
	std::cout << ">> Get by name" << std::endl;
	try {
		std::cout << classes.Get(class_rh.GetLabel()) << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}
	std::cout << ">> Get by ptr" << std::endl;
	try {
		std::cout << classes.Get(&class_rh) << std::endl;
	} catch(TCException e) {
		std::cout << e << std::endl;
	}

	return 0;
}
