#include "tid_SDL_client.h"

#include <sstream>
#include <iostream>
#include <boost/bind.hpp>

#include "input_stream_socket.h"
#include "messages/tid_message_parser_1_0.h"
#include "messages/tid_message_builder_1_0.h"

namespace TiD
{
//-----------------------------------------------------------------------------

TiDClient::TiDClient()
  : state_(State_ConnectionClosed), socket_(io_service_),
    input_stream_(0), msg_parser_(0), msg_builder_(0), receive_thread_(0), io_service_thread_(0)
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
    SDL_WaitThread(io_service_thread_, NULL);
//    delete io_service_thread_;
    io_service_thread_ = 0;
  }

  if(receive_thread_)
  {
    SDL_WaitThread(receive_thread_, NULL);
//    delete receive_thread_;
    receive_thread_ = 0;
  }

  if(input_stream_)
  {
    delete input_stream_;
    input_stream_ = 0;
  }

//  SDL_Quit();

}

//-----------------------------------------------------------------------------

void TiDClient::connect(std::string ip_addr, unsigned int port)
{
  boost::system::error_code ec;

  if(state_ != State_ConnectionClosed)
  {
    std::stringstream ex_str;
    ex_str << "TiAClient: Already connected!" << ip_addr << ":" << port;
    throw(std::invalid_argument( ex_str.str() ));
  }

  boost::asio::ip::address_v4 addr;
  addr.from_string(ip_addr);
  boost::asio::ip::tcp::endpoint peer(addr,port );

  socket_.connect(peer, ec);
  if(ec)
    throw(std::runtime_error("TiDClient::connect -- " + ec.message()) );

  state_ = State_Connected;

  boost::asio::ip::tcp::no_delay delay(true);
  socket_.set_option(delay);
  boost::asio::socket_base::linger linger(true, 0.2);
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

void TiDClient::startReceiving()
{
  if(state_ != State_Connected)
    throw(std::runtime_error("TiDClient::startReceiving -- not connected!") );

  state_ = State_Running;
//  receive_thread_ = new boost::thread(&TiDClient::receive, this);
//  io_service_thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run,
//                                                     &this->io_service_));

  receive_thread_ = SDL_CreateThread(receive, this);
  io_service_thread_ = SDL_CreateThread(run_io_service, this);
}

//-----------------------------------------------------------------------------

void TiDClient::stopReceiving()
{
  state_ = State_Stopped;
}

//-----------------------------------------------------------------------------

int TiDClient::run_io_service(void* instance)
{
  TiDClient* i = static_cast<TiDClient*>(instance);
  i->io_service_.run();
  return 0;
}

//-----------------------------------------------------------------------------

int TiDClient::receive(void* instance)
{
  TiDClient* i = static_cast<TiDClient*>(instance);

  while(i->state_ == State_Running)
  {
    try
    {
      //std::cout << "Trying to get message using an SDL thread ... ";
      IDMessage msg( i->msg_parser_->parseMessage( *i->input_stream_ ) );
      //std::cout << "  got message" << std::endl;

      i->mutex_.lock();
      i->messages_.push_back(msg);
      i->mutex_.unlock();
    }
    catch(std::exception& e)
    {
      if(i->state_ == State_Running)
        std::cerr << e.what() << std::endl << ">> ";
      break;
    }

  }
  return 0;
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
