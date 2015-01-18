/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    It is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.

  TPSocket.hpp/.cpp is adapted from libtransport
*/

#ifndef TPSOCKET_HPP
#define TPSOCKET_HPP

#ifdef _WIN32
#define UNICODE
#define _WIN32_WINNT 0x0501
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <string>

/*! \brief Endpoint structure for TPSocket
 *
 * \ingroup tobiplatform
 *
 * This structure holds whatever turns to be useful to identify an host.
 */
class TPHost {
  public:
    //! \brief Constructor
    TPHost(void);

  public:
    //! Maximum address size
    const static size_t AddressSize = INET_ADDRSTRLEN + 1;
    //! Address (e.g.: 127.0.0.1)
    char address[TPHost::AddressSize];
    //! Port (e.g.: 8000)
    short unsigned int port;
};

/*! \brief TCP/UDP socket
 *
 * \ingroup tobiplatform
 *
 * Provides basic sync/async connectivity.
 */
class TPSocket {
  public:
    /*! \brief Constructor
     * \arg type Socket type, TCP or UDP
     * \arg bsize Internal buffer size
     */
    TPSocket(int type = TPSocket::TCP, size_t bsize = 4096);

    /*! \brief Destructor
     */
    virtual ~TPSocket(void);

    /*! \brief Initializes whatever needs to be initialized
     * \arg asserver Perform initialization for a server socket if true, for
     * a client socket otherwise.
     */
    bool Open(bool asserver);

    /*! \brief Closes the socket
     */
    bool Close(void);

    /*! \brief Configures the socket for asynchronous operation
     * \arg block True for asynchronous operation, false for synchronous
     */
    bool Async(bool block);

    /*! \brief Bind the socket to a specified IP:port
     *
     * If TPSocket is configured AsClient this method will always fail.
     *
     * \arg ip IP address (i.e. "127.0.0.1", "1.2.3.4", "0.0.0.0")
     * \arg port Port to bind (i.e. "8080")
     * \return True if successful, false otherwise
     */
    bool Bind(const std::string &ip, const std::string& port);

    /*! \brief Starts listening
     * \return True if successful, false otherwise
     */
    bool Listen(void);

    /*! \brief Accepts an endpoint
     * \return The file descriptor of successful, <= 0 otherwise
     */
    int Accept(TPSocket* endpoint);

    /*! \brief Connects the socket to a specified IP:port
     *
     * If TPSocket is configured AsServer this method will always fail.
     *
     * \arg ip IP address (i.e. "127.0.0.1", "1.2.3.4", "0.0.0.0")
     * \arg port Port to bind (i.e. "8080")
     * \return True if successful, false otherwise
     */
    bool Connect(const std::string& ip, const std::string& port);

    /*! \brief Sends a string to the remote endpoint
     * \arg message Message to send
     * \return Sent bytes
     */
    ssize_t Send(const std::string& message);

    /*! \brief Receives a string from the remote endpoint
     * \arg message Buffer for receiving
     * \return Received bytes
     */
    ssize_t Recv(std::string* message);

    /*! \brief Checks if socket is connected
     * \return True if connected, false otherwise
     */
    bool IsConnected(void);

#ifdef _WIN32
    static bool InitializeWSA(void);
#endif

  protected:
    //! \brief Allocates internal memory and initializes structures
    void Init(void);
    //! \brief Releases internal memory
    void Free(void);

    //! \brief Updates local TPHost
    bool GetLocal(void);
    //! \brief Updates remote TPHost
    bool GetRemote(void);
    //! \brief Updates maximum buffer size
    void GetMaxBSize(void);

  public:
    //! Host informations associated to the current socket
    TPHost local;

    //! Host informations associated to the remote endpoint
    TPHost remote;

    enum {
      //! Configure as TCP
      TCP = 0,
      //! Configure as UDP
      UDP = 1
    };

  protected:
    //! File descriptor
    int _fd;
    //! Socket address
    struct sockaddr_in _address;
    //! Endpoint address
    struct sockaddr_storage _endpoint;
    //! Buffer
    void* _buffer;
    //! Buffer size
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
#ifdef _WIN32
    static bool _wsaInitialized;
#endif
};

#endif
