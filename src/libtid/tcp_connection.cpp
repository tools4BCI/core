
#include "tcp_connection.h"

namespace TiD
{

//-----------------------------------------------------------------------------

TCPConnection::~TCPConnection()
{
  #ifdef DEBUG
    std::cout << "TCPConnection: ~TCPServer" << std::endl;
  #endif
  socket_.close();
}

//-----------------------------------------------------------------------------

TCPConnection::pointer TCPConnection::create(boost::asio::io_service& io_service)
{
  #ifdef DEBUG
    std::cout << "TCPConnection: create" << std::endl;
  #endif

  return pointer(new TCPConnection(io_service));
}

//-----------------------------------------------------------------------------

std::string TCPConnection::endpointToString(const boost::asio::ip::tcp::endpoint& endpoint)
{
  #ifdef DEBUG
    std::cout << "TCPConnection: endpointToString" << std::endl;
  #endif

  std::ostringstream ss;
  ss << endpoint.address().to_string();
  ss << ":";
  ss << endpoint.port();
  return ss.str();
}

//-----------------------------------------------------------------------------

} //TiD