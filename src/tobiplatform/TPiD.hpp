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

#ifndef TPID_HPP 
#define TPID_HPP 

#include "TPInterface.hpp"
#include <tobiid/IDSerializer.hpp>
#include <tobicore/TCLanguage.hpp>

/*! \brief TOBI iD interface
 */
class TPiD : public TPInterface {
	public:
		TPiD(void);
		virtual ~TPiD(void);
		int Set(IDSerializer* serializer, int bidx = TCBlock::BlockIdxUnset, 
				int* abidx = NULL);
		int Get(IDSerializer* serializer);
	private:
		virtual int ConfAsClient(const std::string &ip, const std::string& port);
		virtual int ConfAsServer(const std::string &ip, const std::string& port);
	private:
		TCLanguage _lang;
};

#endif
