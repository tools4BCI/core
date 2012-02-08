#include "tid_server.h"

#include <boost/bind.hpp>
#include <iostream>

// local

#include "tobiid/IDMessage.hpp"

//#include "messages/tid_message.h"

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

namespace TiD
{

//-----------------------------------------------------------------------------

TiDServer::TiDServer(boost::asio::io_service& io_service)
  : TCPServer(io_service), running_(1)
{
  #ifdef DEBUG
    std::cout << "TiDServer: Constructor" << std::endl;
  #endif

  messages_.reserve(100);
}

//-----------------------------------------------------------------------------

TiDServer::~TiDServer ()
{
  #ifdef DEBUG
    std::cout << "TiDServer: ~TiDServer" << std::endl;
  #endif

  running_ = 0;
  for(TiDConnHandlers::iterator it( connections_.begin() );
      it != connections_.end(); it++)
  {
    it->second->stop();
  }
  erase_mutex_.lock();
  connections_.clear();
  erase_mutex_.unlock();
  dispatch_mutex_.lock();
  messages_.clear();
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

bool TiDServer::newMessagesAvailable()
{
  #ifdef DEBUG
    std::cout << "TiDServer: newMessagesAvailable" << std::endl;
  #endif

  bool available = false;
  dispatch_mutex_.lock();
  available = messages_.size();
  dispatch_mutex_.unlock();

  return available;
}

//-----------------------------------------------------------------------------

void TiDServer::getLastMessages(std::vector<IDMessage>& messages)
{
  #ifdef DEBUG
    std::cout << "TiDServer: getLastMessages" << std::endl;
  #endif

  dispatch_mutex_.lock();
  messages = messages_;
  messages_.clear();
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::handleAccept(const TCPConnection::pointer& new_connection,
      const boost::system::error_code& error)
{
  #ifdef DEBUG
    std::cout << "TiDServer: handleAccept" << std::endl;
  #endif

  if (error)
  {
    // TODO: error handling
    return;
  }

  deleteConnectionCallback del_con_cb = boost::bind( &TiDServer::clientHasDisconnected, this, _1);
  dispatchTiDMessageCallback disp_msg_cb = boost::bind( &TiDServer::dispatchMsg, this, _1, _2);

  TiDConnection::pointer connection = TiDConnection::create(new_connection,
                                                            del_con_cb, disp_msg_cb );
  TiDConnection::ConnectionID id = connection->getID();

  connections_.insert(make_pair(id, connection));

  cout << "  ** TiD Server: Client @" << id.second << ":" << id.first << " has connected.";
  cout << " -- Connected clients: " << connections_.size() << endl;

  connection->run();
  startAccept();
}

//-----------------------------------------------------------------------------

void TiDServer::clientHasDisconnected(const TiDConnection::ConnectionID& id)
{
  #ifdef DEBUG
    std::cout << "TiDServer: clientHasDisconnected" << std::endl;
  #endif

  if(!running_)
    return;

  erase_mutex_.lock();
  std::cout << "  ** TiD Server: Connection to client @";
  cout << id.second << ":" << id.first << " has been closed.";
  connections_.erase(id);
  std::cout << " -- Connected clients: " << connections_.size() << std::endl << std::flush;

  erase_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::dispatchMsg(IDMessage msg, const TiDConnection::ConnectionID& src_id)
{
  #ifdef DEBUG
    std::cout << "TiDServer: dispatchMsg" << std::endl;
  #endif

  dispatch_mutex_.lock();
  messages_.push_back(msg);

  for(TiDConnHandlers::iterator it( connections_.begin() );
      it != connections_.end(); it++)
  {
    if(src_id.first != it->first.first )
      it->second->sendMsg(msg);
  }
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

}
