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

#ifndef TPINTERFACE_HPP 
#define TPINTERFACE_HPP 

#include "TPSocket.hpp"
#include "TPStreamer.hpp"
#include <string>

class TPInterface {
	public:
		TPInterface(void);
		virtual ~TPInterface(void);
		int Plug(const std::string &ip, const std::string& port,
				int mode = TPInterface::AsServer);
		void Unplug(void);
		bool IsPlugged(void);
	private:
		virtual int ConfAsServer(const std::string &ip, const std::string& port);
		virtual int ConfAsClient(const std::string &ip, const std::string& port);

	public:
		const static int AsServer = 0;
		const static int AsClient = 1;
		const static int Unsuccessful = -1;
		const static int Successful = 0;
		const static int ErrorSocket = 1;
		const static int ErrorEndpoint = 2;
		const static int ErrorBound = 3;
		const static int ErrorGeneric = 4;
		const static int ErrorNotSupported = 5;
	protected:
		TPSocket* _socket;
		TPSocket* _endpoint;
		TPSocket* _com;
		TPStreamer _stream;
		std::string _cache;
};

#endif
