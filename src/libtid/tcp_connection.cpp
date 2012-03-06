
#include "tcp_connection.h"
#include <boost/current_function.hpp>

namespace TiD
{

//-----------------------------------------------------------------------------

TCPConnection::~TCPConnection()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif
  socket_.close();
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