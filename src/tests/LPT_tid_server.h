#ifndef LPT_TID_SERVER_H
#define LPT_TID_SERVER_H

#include "libtid/tid_server.h"
#include "LPT_tid_connection.h"

namespace TiD
{

class LPTTiDServer : public TiD::TiDServer
{
  public:
    LPTTiDServer()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }

    ~LPTTiDServer()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }


 protected:
  virtual void handleAccept(const TCPConnection::pointer& new_connection,
      const boost::system::error_code& error)
  {
    #ifdef DEBUG
      std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
    #endif

    if (error)
    {
      // TODO: error handling
      return;
    }

    if(!running_)
      return;

    boost::asio::socket_base::send_buffer_size send_buffer_option(SOCKET_BUFFER_SIZE);
    new_connection->socket().set_option(send_buffer_option);

    boost::asio::socket_base::receive_buffer_size recv_buffer_option(SOCKET_BUFFER_SIZE);
    new_connection->socket().set_option(recv_buffer_option);

    boost::asio::ip::tcp::no_delay delay_option(true);
    new_connection->socket().set_option(delay_option);

    boost::asio::socket_base::linger linger_option( (SOCKET_LINGER_TIMEOUT >0),
                                                    SOCKET_LINGER_TIMEOUT);
    new_connection->socket().set_option(linger_option);

    erase_mutex_.lock();
    deleteConnectionCallback del_con_cb = boost::bind( &TiDServer::clientHasDisconnected, this, _1);
    dispatchTiDMessageCallback disp_msg_cb = boost::bind( &TiDServer::dispatchMsg, this, _1, _2);

    LPTTiDConnection::pointer connection = LPTTiDConnection::create(new_connection,
                                                              del_con_cb, disp_msg_cb );
    LPTTiDConnection::ConnectionID id = connection->getID();

    connections_.insert(make_pair(id, connection));

    std::cout << "  ** TiD Server: Client @" << id.second << ":" << id.first << " has connected.";
    std::cout << " -- Connected clients: " << connections_.size() << std::endl;

    connection->run();
    startAccept();
    erase_mutex_.unlock();
  }

};

}
#endif // TIMED_TID_SERVER_H
