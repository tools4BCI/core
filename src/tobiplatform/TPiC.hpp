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

#ifndef TPIC_HPP
#define TPIC_HPP

#include "TPInterface.hpp"
#include <tobiic/ICSerializer.hpp>

/*! \brief TOBI iC interface
 *
 * \ingroup tobiplatform
 *
 * Implements a TOBI iC client or sever object by heavily relying on the
 * functionalities provided by TPInterface.
 */
class TPiC : public TPInterface {
  public:
    /*! \brief Sends an ICMessage
     *
     * An ICSerializer is configured passing a reference to an ICMessage.
     * This method simply needs a reference to an existing ICSerializer to
     * serialize its content and send it over a TCP connection.
     *
     * \arg serializer Reference to ICSerializer
     */
    int Set(ICSerializer* serializer);

    /*! \brief Receives an ICMessage
     *
     * An ICSerializer is configured passing a reference to an ICMessage.
     * This method simply needs a reference to an existing ICSerializer to
     * deserialize a message received via a TCP connection.
     *
     * \arg serializer Reference to ICSerializer
     */
    int Get(ICSerializer* serializer);
};

#endif
