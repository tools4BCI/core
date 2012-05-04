
#include "tcp_connection.h"
#include <boost/current_function.hpp>
#include <iostream>

namespace TiD
{

//-----------------------------------------------------------------------------

TCPConnection::~TCPConnection()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
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
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  return pointer(new TCPConnection(io_service));
}

//-----------------------------------------------------------------------------

std::string TCPConnection::endpointToString(const boost::asio::ip::tcp::endpoint& endpoint)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
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
