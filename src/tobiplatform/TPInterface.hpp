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
*/

#ifndef TPINTERFACE_HPP
#define TPINTERFACE_HPP

#include "TPSocket.hpp"
#include "TPStreamer.hpp"
#include <string>

/*! \brief TOBI platform generic interface
 *
 * \ingroup tobiplatform
 *
 * This class provides all the methods to develop a TCP/IP TOBI platform
 * component. It uses a TPSocket object to abstract communication as server or
 * as client.
 * For this reason, a TPInterface can be "plugged" to another. The TOBI platform
 * iC and iD pluggable classes (TPiC and TPiD) derive directly from this class.
 *
 * \sa TPiC, TPiD
 */
class TPInterface {
  public:
    /*! \brief Constructor
     */
    TPInterface(void);

    /*! \brief Destructor
     */
    virtual ~TPInterface(void);

    /*! \brief Plug
     *
     * This method allows to bind a TCP socket or to connect to a
     * remote one.
     *
     * \arg ip IP to bind or remote IP to connect to.
     * \arg port Port to bind or remote port to connect to.
     * \arg mode Behavior, AsClient or AsServer
     * \return Status or error code
     */
    int Plug(const std::string &ip, const std::string& port,
        int mode = TPInterface::AsServer);

    /*! \brief Unplug
     *
     * Unbinds or disconnect.
     */
    void Unplug(void);

    bool IsPlugged(void);
  protected:
    virtual int ConfAsServer(const std::string &ip, const std::string& port);

    virtual int ConfAsClient(const std::string &ip, const std::string& port);

  public:
    /*! \brief Plugging mode
     */
    enum {
      //! Configure/plug as TCP server
      AsServer = 0,

      //! Configure/plug as TCP client
      AsClient
    };

    /*! \brief Status/error codes.
     */
    enum {
      //! Generic unsuccessful operation
      Unsuccessful = 0,

      //! Generic uccessful operation
      Successful,

      //! Cannot bind, listen or connect socket
      ErrorSocket,

      //! Cannot communicate with endpoint
      ErrorEndpoint,

      //! Socket already bound/connected
      ErrorBound,

      //! Generic configuration error
      ErrorGeneric,

      //! Operattion/request not supported
      ErrorNotSupported,

      //! Protocol related error
      ErrorProtocol,

      //! Timeout error
      ErrorTimeout
    };
  protected:
    //! \brief Socket for TCP communication
    TPSocket* _socket;

    //! \brief Reference to communication endpoint
    TPSocket* _endpoint;

    //! \brief Reference to _socket or _endpoint
    TPSocket* _com;

    //! \brief Data streamer
    TPStreamer _stream;

    //! \brief
    std::string _cache;
};

#endif
