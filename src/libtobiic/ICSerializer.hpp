/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiic library

    The libtobiid library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ICSERIALIZER_HPP
#define ICSERIALIZER_HPP

#include "ICMessage.hpp"

class ICSerializer {
	public:
		ICSerializer(void);
		ICSerializer(ICMessage* const message);
		virtual ~ICSerializer(void);
		
		ICSerializer* SetMessage(ICMessage* const message);

		virtual std::string* Serialize(std::string* buffer) = 0;
		virtual std::string* Deserialize(std::string* const buffer) = 0;
		
		virtual std::string SerializeCp(void);
		virtual void DeserializeCp(const std::string& buffer);
		
		virtual char* SerializeCh(char* buffer, const unsigned int bsize);
		virtual const char* DeserializeCh(const char* buffer, const unsigned int bsize);
	
	public:
		ICMessage* message;
};

#endif
