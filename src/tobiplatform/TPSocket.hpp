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

	TPSocket.hpp/.cpp is adapted from libtransport
*/

#ifndef TPSOCKET_HPP 
#define TPSOCKET_HPP 

#include <arpa/inet.h>
#include <netdb.h>
#include <string>

/*! \brief libtransport host 
 * This structure holds whatever identifies an host
 */
class TPHost {
	public:
		TPHost(void);

	public:
		const static size_t AddressSize = INET_ADDRSTRLEN + 1;
		//! Address (e.g.: 127.0.0.1)
		char address[TPHost::AddressSize];
		//! Port (e.g.: 8000)
		short unsigned int port;
};

/*! \brief TCP/UDP socket
 */
class TPSocket {
	public:
		TPSocket(int type = TPSocket::TCP, size_t bsize = 4096);
		virtual ~TPSocket(void);
		bool Open(bool asserver);
		bool Close(void);
		bool Bind(const std::string &ip, const std::string& port);
		bool Listen(void);
		int Accept(TPSocket* endpoint);
		bool Connect(const std::string& ip, const std::string& port);
		ssize_t Send(const std::string& message);
		ssize_t Recv(std::string* message);

	protected:
		void Init(void);
		void Free(void);
		bool GetLocal(void);
		bool GetRemote(void);
		void GetMaxBSize(void);

	public:
		//! Host informations associated to the current socket
		TPHost local;
		TPHost remote;
		const static int TCP = 0;
		const static int UDP = 1;

	protected:
		//! File descriptor
		int _fd;
		//! Socket address
		struct sockaddr_in _address;
		struct sockaddr_storage _endpoint;
		//! Buffer 
		void* _buffer;
		size_t _bsize;
		//! Maximum number of connections
		unsigned int _mc;
		//! Socket info
		struct addrinfo _results;
		//! Socket info
		struct addrinfo* _info;
		//! Maximum size of message
		size_t _bsizemax;
	private:
		//! Socket type
		int _type;
};

#endif
