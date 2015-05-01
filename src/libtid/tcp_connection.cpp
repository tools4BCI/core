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

#include "tcp_connection.h"
#include <boost/current_function.hpp>
#include <iostream>
#include <thread>

namespace TiD
{

//-----------------------------------------------------------------------------

TCPConnection::~TCPConnection()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::system::error_code error;
  socket_.close(error);
  if(error == boost::asio::error::bad_descriptor)
    return;
  if(error == boost::asio::error::not_connected)
    return;
  else if(error)
    std::cerr << BOOST_CURRENT_FUNCTION << " -- " << error << std::endl;
}

//-----------------------------------------------------------------------------

TCPConnection::pointer TCPConnection::create(boost::asio::io_service& io_service)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  return pointer(new TCPConnection(io_service));
}

//-----------------------------------------------------------------------------

std::string TCPConnection::endpointToString(const boost::asio::ip::tcp::endpoint& endpoint)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  std::ostringstream ss;
  ss << endpoint.address().to_string();
  ss << ":";
  ss << endpoint.port();
  return ss.str();
}

//-----------------------------------------------------------------------------

TCPConnection::TCPConnection(boost::asio::io_service& io_service)
  : socket_(io_service)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif
}

//-----------------------------------------------------------------------------

} //TiD
