/*
    This file is part of TOBI Interface D (TiD).

    TOBI Interface D (TiD) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TOBI Interface D (TiD) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TOBI Interface D (TiD).  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#ifndef INPUT_STREAM_SOCKET_H
#define INPUT_STREAM_SOCKET_H

#include "input_stream.h"
#include "tid_exceptions.h"

#include <boost/asio.hpp>
#include <string>

namespace TiD
{

//-----------------------------------------------------------------------------
/// base class for any socket
class InputStreamSocket : public InputStream
{
  friend class TimedInputStreamSocket;

  public:
    InputStreamSocket (boost::asio::ip::tcp::socket& socket);
    virtual ~InputStreamSocket ();

    virtual void readUntil (const std::string& delimiter, std::string* str);

//    virtual void readUntil (std::string* str);
//    virtual void setDelimiter(const std::string& del);

    virtual void fustyreadUntil (const std::string& delimiter, std::string* str);

  private:
    bool parseIstream(std::istream& stream, std::string* out_string, const std::string& delimiter);

  //    void readLineHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);
  //    void readStringHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);


  private:
    std::string*                         str_buffer_;
    std::string*                         tmp_str_;

    boost::asio::ip::tcp::socket&        socket_;
    boost::system::error_code            error_;


    // just used within the fusty version -- will be removed in future
    boost::asio::streambuf               stream_buffer_;


    size_t                               last_pos_;
//    std::string                          delimiter_;
};

}

#endif // INPUT_STREAM_SOCKET_H
