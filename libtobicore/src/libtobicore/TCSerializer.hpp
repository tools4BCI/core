/*
    Copyright (C) 2010  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This file is part of the libtobiic library

    The libtobiic library is free software: you can redistribute it and/or
    modify it under the terms of the version 3 of the GNU General Public
    License as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTTCULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TCSERIALIZER_HPP
#define TCSERIALIZER_HPP

#include "TCBlock.hpp"

class TCSerializer {
	public:
		TCSerializer(void);
		TCSerializer(TCBlock* const block);
		virtual ~TCSerializer(void);
		
		TCSerializer* SetMessage(TCBlock* const block);

		virtual std::string* Serialize(std::string* buffer) = 0;
		virtual std::string* Deserialize(std::string* const buffer) = 0;
		
		virtual std::string SerializeCp(void);
		virtual void DeserializeCp(const std::string& buffer);
		
		virtual char* SerializeCh(char* buffer, const unsigned int bsize);
		virtual const char* DeserializeCh(const char* buffer, const unsigned int bsize);
	
	protected:
		TCBlock* _message;
};

#endif
