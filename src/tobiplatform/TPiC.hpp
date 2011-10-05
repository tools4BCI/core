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

#ifndef TPIC_HPP 
#define TPIC_HPP 

#include "TPSocket.hpp"

/*! \brief Simple iC interface
 */
class TPiC {
	public:
		TPiC(void);
		virtual ~TPiC(void);
		bool Send(void);
		bool Recv(void);
		int Plug(const std::string &ip, const std::string& port,
				int mode = TPiC::AsServer);
		void Unplug(void);
	private:
		int ConfAsServer(const std::string &ip, const std::string& port);
		int ConfAsClient(const std::string &ip, const std::string& port);

	public:
		const static int AsServer = 0;
		const static int AsClient = 1;
		const static int Successful = 0;
		const static int ErrorSocket = 1;
		const static int ErrorEndpoint = 2;
		const static int ErrorBound = 3;
		const static int ErrorGeneric = 4;

	private:
		TPSocket* _socket;
		TPSocket* _endpoint;
	//protected:
	public:
		TPSocket* com;
};

#endif
