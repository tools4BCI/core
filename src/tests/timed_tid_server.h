/*
    This file is part of the libTiD test routine.

    The libTiD test routine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The libTiD test routine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the libTiD test routine. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#ifndef TIMED_TID_SERVER_H
#define TIMED_TID_SERVER_H

#include <boost/chrono.hpp>
#include <list>

#include "libtid/tid_server.h"
#include "timed_tid_connection.h"

namespace TiD
{

class TimedTiDServer : public TiD::TiDServer
{
  public:
    TimedTiDServer()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }

    ~TimedTiDServer()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }

    std::list< std::vector<double> > getConnectionDispatchTimings()
    {
      std::list< std::vector<double> > timings;
      for(TiDConnHandlers::iterator it( connections_.begin() );
          it != connections_.end(); it++)
      {
        TimedTiDConnection* srv = dynamic_cast<TimedTiDConnection*>(it->second.get());
        timings.push_back(srv->getRecvTimingValues() );
      }
      return(timings);
    }

    void clearConnectionDispatchTimings()
    {
      for(TiDConnHandlers::iterator it( connections_.begin() );
          it != connections_.end(); it++)
      {
        TimedTiDConnection* srv = dynamic_cast<TimedTiDConnection*>(it->second.get());
        srv->clearRecvTimingValues();
      }
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

    TimedTiDConnection::pointer connection = TimedTiDConnection::create(new_connection,
                                                              del_con_cb, disp_msg_cb );
    TimedTiDConnection::ConnectionID id = connection->getID();

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
