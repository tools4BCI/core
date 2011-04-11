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

#ifndef TCTIMESTAMP_HPP 
#define TCTIMESTAMP_HPP 
#include <sys/time.h>
#include <string>

typedef struct timeval TCTimeval;

class TCTimestamp {
	public:
		TCTimestamp(void);
		virtual ~TCTimestamp(void);
		virtual void Unset(void);
		virtual bool IsSet(void);
		virtual void Tic(void);
		virtual double Toc(TCTimeval* timestamp);
		virtual double Toc(void);
		virtual void Get(std::string* timestamp) const;
		virtual bool Set(const std::string& timestamp);
		virtual void Set(const TCTimeval* timestamp);
	
	public:
		TCTimeval timestamp;
};

#endif
