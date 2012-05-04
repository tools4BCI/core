#include "tid_server.h"

#include <boost/bind.hpp>
#include <iostream>
#include <math.h>

// local

#include "tobiid/IDMessage.hpp"
#include "messages/tid_message_builder_1_0.h"


using std::cout;
using std::cerr;
using std::endl;
using std::flush;

namespace TiD
{

//-----------------------------------------------------------------------------

TiDServer::TiDServer()
  : running_(0), current_rel_timestamp_(0), current_packet_nr_(0)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  messages_.reserve(100);
  msg_builder_  = new TiDMessageBuilder10();
  current_xml_string_.reserve(2048);
}

//-----------------------------------------------------------------------------

TiDServer::~TiDServer ()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(running_)
    stop();

  if(msg_builder_)
    delete(msg_builder_);
}

//-----------------------------------------------------------------------------

void TiDServer::start()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif
  listen();
  running_ = true;
}

//-----------------------------------------------------------------------------

void TiDServer::stop()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif
  running_ = false;

  for(TiDConnHandlers::iterator it( connections_.begin() );
      it != connections_.end(); it++)
  {
//    it->second->socket()
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
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
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
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  dispatch_mutex_.lock();
  messages = messages_;
  messages_.clear();
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::clearMessages()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  dispatch_mutex_.lock();
  messages_.clear();
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::reserveNrOfMsgs (size_t expected_nr_of_msgs)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  dispatch_mutex_.lock();
  messages_.reserve(expected_nr_of_msgs);
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::handleAccept(const TCPConnection::pointer& new_connection,
      const boost::system::error_code& error)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if (error)
  {
    std::cerr << BOOST_CURRENT_FUNCTION << error << std::endl;
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

  #ifndef WIN32
    int i = 1;
    setsockopt( new_connection->socket().native_handle(), IPPROTO_TCP, TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif

  erase_mutex_.lock();
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
  erase_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::clientHasDisconnected(const TiDConnection::ConnectionID& id)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(!running_)
    return;

  dispatch_mutex_.lock();
  erase_mutex_.lock();
  //std::cout << "  ** TiD Server: Connection to client @";
  //cout << id.second << ":" << id.first << " has been closed.";
  connections_.erase(id);
  //std::cout << " -- Connected clients: " << connections_.size() << std::endl << std::flush;

  erase_mutex_.unlock();
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::dispatchMsg(IDMessage& msg, const TiDConnection::ConnectionID& src_id)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(!running_)
    return;

  // TODO: Maybe increase speed with multithreaded dispatching if needed!

  if( (msg.GetBlockIdx() == TCBlock::BlockIdxUnset) && current_packet_nr_)
  {
    msg.SetBlockIdx(current_packet_nr_);
    current_timeval_.tv_sec = floor(current_rel_timestamp_ * 1000000.0);
    current_timeval_.tv_usec = current_rel_timestamp_ - (current_timeval_.tv_sec * 1000000);
    msg.relative.Set(&current_timeval_);
  }

  msg_builder_->buildTiDMessage(msg, current_xml_string_);

  dispatch_mutex_.lock();
  for(TiDConnHandlers::iterator it( connections_.begin() );
      it != connections_.end(); it++)
  {
    if(src_id.first != it->first.first )
      it->second->sendMsg(current_xml_string_);
    io_service_.poll();
  }
  messages_.push_back(msg);
  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDServer::update(boost::uint64_t rel_timestamp, boost::uint64_t packet_nr)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  dispatch_mutex_.lock();

  current_rel_timestamp_ = rel_timestamp;
  current_packet_nr_ = packet_nr;

  dispatch_mutex_.unlock();
}

//-----------------------------------------------------------------------------

}
