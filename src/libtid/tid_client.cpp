#include "tid_client.h"

#include <sstream>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/exception/all.hpp>

#include "input_stream_socket.h"
#include "messages/tid_message_parser_1_0.h"
#include "messages/tid_message_builder_1_0.h"

namespace TiD
{
//-----------------------------------------------------------------------------

TiDClient::TiDClient()
  : state_(State_ConnectionClosed), socket_(io_service_),
    input_stream_(0), msg_parser_(0), msg_builder_(0), receive_thread_(0), 
    io_service_thread_(0), throw_on_error_(0)
{
  input_stream_ = new InputStreamSocket(socket_);
  msg_parser_   = new TiDMessageParser10();
  msg_builder_  = new TiDMessageBuilder10();

  messages_.reserve(100);

//  io_service_thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run,
//                                                     &this->io_service_));
}

//-----------------------------------------------------------------------------

TiDClient::~TiDClient()
{
  io_service_.stop();
  disconnect();

  if(msg_builder_)
  {
    delete msg_builder_;
    msg_builder_ = 0;
  }

  if(msg_parser_)
  {
    delete msg_parser_;
    msg_parser_ = 0;
  }

  if(io_service_thread_)
  {
    io_service_thread_->join();
    delete io_service_thread_;
    io_service_thread_ = 0;
  }

  if(receive_thread_)
  {
    receive_thread_->interrupt();
    receive_thread_->join();
    delete receive_thread_;
    receive_thread_ = 0;
  }

  if(input_stream_)
  {
    delete input_stream_;
    input_stream_ = 0;
  }
}

//-----------------------------------------------------------------------------

void TiDClient::connect(std::string ip_addr, unsigned int port)
{
  boost::system::error_code ec;

  if(state_ != State_ConnectionClosed)
  {
    std::stringstream ex_str;
    ex_str << "TiDClient: Already connected!" << ip_addr << ":" << port;
    throw(std::invalid_argument( ex_str.str() ));
  }

  boost::asio::ip::tcp::endpoint peer(boost::asio::ip::address::from_string(ip_addr),port );

  socket_.connect(peer, ec);
  if(ec)
    throw(std::runtime_error("TiDClient::connect -- " + ec.message()) );

  state_ = State_Connected;

  boost::asio::ip::tcp::no_delay delay(true);
  socket_.set_option(delay);
  boost::asio::socket_base::linger linger(true, 1);
  socket_.set_option(linger);

//  boost::asio::socket_base::non_blocking_io command(true);
//  socket_.io_control(command);
}

//-----------------------------------------------------------------------------

void TiDClient::disconnect()
{
  if(state_ == State_ConnectionClosed)
    return;

  socket_.cancel();
  boost::system::error_code ec;  //ignored
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_.close(ec);

  state_ = State_ConnectionClosed;
}

//-----------------------------------------------------------------------------

void TiDClient::setBufferSize(size_t size)
{
  boost::asio::socket_base::receive_buffer_size recv_buffer_size( size );
  socket_.set_option(recv_buffer_size);

  boost::asio::socket_base::receive_buffer_size send_buffer_size( size );
  socket_.set_option(send_buffer_size);
}

//-----------------------------------------------------------------------------

void TiDClient::sendMessage(std::string tid_xml_context)
{
  boost::asio::async_write(socket_, boost::asio::buffer(tid_xml_context),
                           boost::bind(&TiDClient::handleWrite, this,
                                       boost::asio::placeholders::error, tid_xml_context.size() ));
}

//-----------------------------------------------------------------------------

void TiDClient::startReceiving(bool throw_on_error)
{
  throw_on_error_ = throw_on_error;

  if(state_ != State_Connected)
    throw(std::runtime_error("TiDClient::startReceiving -- not connected!") );

  state_ = State_Running;
  receive_thread_ = new boost::thread(&TiDClient::receive, this);
  io_service_thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run,
                                                     &this->io_service_));
}

//-----------------------------------------------------------------------------

void TiDClient::stopReceiving()
{
  state_ = State_Stopped;
}

//-----------------------------------------------------------------------------

void TiDClient::receive()
{
  while(state_ == State_Running)
  {
    try
    {
      std::cout << "Trying to get message" << std::endl;
      IDMessage msg( msg_parser_->parseMessage( *input_stream_ ) );
      std::cout << "Got message" << std::endl;

      mutex_.lock();
      messages_.push_back(msg);
      mutex_.unlock();
    }
    catch(std::exception& e)
    {
      if(throw_on_error_)
        boost::throw_exception(e);

      if(state_ == State_Running)
        std::cerr << e.what() << std::endl << ">> ";
      state_ = State_Error;
      return;
    }

  }
}

//-----------------------------------------------------------------------------

void TiDClient::handleWrite(const boost::system::error_code &ec,
                              std::size_t /*bytes_transferred*/)
{
  if(ec)
    throw(std::runtime_error("TiDClient::handleWrite -- " + ec.message()) );
}

//-----------------------------------------------------------------------------

void TiDClient::getLastMessagesContexts(std::vector< IDMessage >& messages )
{
  mutex_.lock();
  messages = messages_;
  messages_.clear();
  mutex_.unlock();
}

//-----------------------------------------------------------------------------

bool TiDClient::newMessagesAvailable()
{
  bool available = false;
  mutex_.lock();
  available = messages_.size();
  mutex_.unlock();

  return available;
}

//-----------------------------------------------------------------------------

}  //TiD
