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

#ifndef TPID_HPP
#define TPID_HPP

#include "TPInterface.hpp"
#include <tobiid/IDSerializer.hpp>
#include <tobicore/TCLanguage.hpp>

/*! \brief TOBI iD interface
 *
 * \ingroup tobiplatform
 *
 * Implements a TOBI iD client relying on the functionalities provided by
 * TPInterface.
 */
class TPiD : public TPInterface {
  public:
    //! \brief Constructor
    TPiD(void);

    //! \brief Destructor
    virtual ~TPiD(void);

    /*! \brief Sends an IDMessage
     *
     * An IDSerializer is configured passing a reference to an IDMessage.
     * This method simply needs a reference to an existing IDSerializer to
     * serialize its content and send it over a TCP connection.
     * Each time we send an IDMessage to the acquisition server we need to
     * tag the aformentioned message with a frame number. Once the server
     * received the IDMessage, it replies to the sender by sending a status
     * message containg the frame number at which the message was received.
     *
     * \arg serializer Reference to ICSerializer
     * \arg bidx Frame number
     * \arg abidx Frame number at which the message was received
     * \return TPInterface::Successful upon success
     *
     * \sa TCBlock, TCLanguage
     */
    int Set(IDSerializer* serializer, int bidx = TCBlock::BlockIdxUnset,
        int* abidx = NULL);

    /*! \brief Receives an IDMessage
     *
     * An IDSerializer is configured passing a reference to an IDMessage.
     * This method simply needs a reference to an existing IDSerializer to
     * serialize its content and send it over a TCP connection.
     *
     * \arg serializer Reference to ICSerializer
     * \arg bidx Frame number
     * \arg abidx Frame number at which the message was received
     * \return TPInterface::Successful upon success
     *
     * \sa TCBlock, TCLanguage
     */
    int Get(IDSerializer* serializer);
  private:

    /*! \brief Implements specific configuration
     *
     * Overrides TPInterface::ConfAsClient
     */
    virtual int ConfAsClient(const std::string &ip, const std::string& port);

    /*! \brief Implements specific configuration
     *
     * Overrides TPInterface::ConfAsServer
     */
    virtual int ConfAsServer(const std::string &ip, const std::string& port);
  private:

    TCLanguage _lang;

    TPStreamer _sendrecv;
};

#endif
