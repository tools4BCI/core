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

#include "TCTools.hpp"
#include "TCException.hpp"
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <sstream>

#ifdef __BORLANDC__
using namespace std;
#endif

char* TCTools::itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

int TCTools::ftoa(float value, char* result) {
	return sprintf(result, "%.6f", value);
}

float TCTools::atof(const char* value) {
	float retval = 0.00f;

	int fields = sscanf(value, "%f", &retval);
	if(fields != 1)
		throw TCException("Field number exceeded", 
												                       #ifdef _WIN32  
															                         __FUNCSIG__       
												                       #else          
                                                       __PRETTY_FUNCTION__ 
												                       #endif 
		);

	return retval;
}

std::string TCTools::itos(int value) {
	std::stringstream out;
	out << value;
	return out.str();
}
