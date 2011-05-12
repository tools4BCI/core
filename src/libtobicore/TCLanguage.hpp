/*
    Copyright (C) 2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobicore library

    The libtobicore library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTTCULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TCLANGUAGE_HPP 
#define TCLANGUAGE_HPP 

#include <string>

class TCLanguage {
	public:
		std::string Status(const int component, const int status);
		bool CheckVersion(const std::string& message);
		bool IsStatus(const std::string& message, int* component, int* status);
		
		std::string GetComponent(int component);
		std::string GetStatus(int component);

	public:
		const static int IA = 1;
		const static int IB = 2;
		const static int IC = 3;
		const static int ID = 4;

		const static int Ready = 1;
		const static int Quit  = 2;
		const static int ErrorGeneric = -1;
};

#endif
