#include "tid_client.h"

#include <sstream>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/exception/all.hpp>
#include <boost/current_function.hpp>

#include "input_stream_socket.h"
#include "messages/tid_message_parser_1_0.h"
#include "messages/tid_message_builder_1_0.h"


#ifdef LPT_TEST
  #include "LptTools/LptTools.h"
  #define LPT1  0
  #define LPT2  1
#endif

namespace TiD
{

static const int SOCKET_BUFFER_SIZE = 4194304;

//-----------------------------------------------------------------------------

TiDClient::TiDClient()
  : state_(State_ConnectionClosed), socket_(io_service_),
    input_stream_(0), msg_parser_(0), msg_builder_(0), receive_thread_(0),
    io_service_thread_(0), throw_on_error_(0)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  input_stream_ = new InputStreamSocket(socket_);
  msg_parser_   = new TiDMessageParser10();
  msg_builder_  = new TiDMessageBuilder10();

  messages_.reserve(100);
  xml_string_.reserve(2048);

  #ifdef LPT_TEST
    lpt_flag_ = 0;
    if(!LptDriverInstall())
      std::cerr << "Installing LptTools lpt driver failed (do you have access rights for the lpt-port?)." << std::endl;

    if(!LptInit())
      std::cerr << "Initializing lpt driver failed (do you have access rights for the lpt-port?)." << std::endl;
  #endif
}

//-----------------------------------------------------------------------------

TiDClient::~TiDClient()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

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
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

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
  boost::asio::socket_base::linger linger(true, 0);
  socket_.set_option(linger);
  boost::asio::socket_base::send_buffer_size send_buffer_option(SOCKET_BUFFER_SIZE);
  socket_.set_option(send_buffer_option);
  boost::asio::socket_base::receive_buffer_size recv_buffer_option(SOCKET_BUFFER_SIZE);
  socket_.set_option(recv_buffer_option);
}

//-----------------------------------------------------------------------------

void TiDClient::disconnect()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(state_ == State_ConnectionClosed)
    return;

  boost::system::error_code ec;  //ignored
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_.close(ec);

  state_ = State_ConnectionClosed;
}

//-----------------------------------------------------------------------------

void TiDClient::setBufferSize(size_t size)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::asio::socket_base::receive_buffer_size recv_buffer_size( size );
  socket_.set_option(recv_buffer_size);

  boost::asio::socket_base::receive_buffer_size send_buffer_size( size );
  socket_.set_option(send_buffer_size);
}

//-----------------------------------------------------------------------------

void TiDClient::sendMessage(std::string& tid_xml_context)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::asio::async_write(socket_, boost::asio::buffer(tid_xml_context),
                           boost::bind(&TiDClient::handleWrite, this,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred ));
}

//-----------------------------------------------------------------------------

void TiDClient::sendMessage(IDMessage& msg)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  #ifdef TIMING_TEST
    msg.absolute.Tic();
    msg.relative.Tic();
  #endif

  #ifdef LPT_TEST
    int port_state = LptPortIn(LPT1,0);
    if(!lpt_flag_)
    {
      lpt_flag_ = 1;
      LptPortOut(LPT1, 0, port_state | 0x01);
    }
    else
    {
      lpt_flag_ = 0;
      LptPortOut(LPT1, 0, port_state & ~0x01);
    }
  #endif

  msg_builder_->buildTiDMessage(msg, xml_string_);

  #ifdef TIMING_TEST
    double abs_diff = msg.absolute.Toc();
    double rel_diff = msg.relative.Toc();
    std::cout << "  ** TIMING TEST ** -- time to bild msg -- abs/rel time-diff:  " << abs_diff <<"/"<< rel_diff;
    std::cout << std::endl << std::flush;
  #endif

  boost::asio::async_write(socket_, boost::asio::buffer(xml_string_),
                           boost::bind(&TiDClient::handleWrite, this,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred ));

  xml_string_.clear();
}

//-----------------------------------------------------------------------------

void TiDClient::startReceiving(bool throw_on_error)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  throw_on_error_ = throw_on_error;

  if(state_ != State_Connected)
    throw(std::runtime_error("TiDClient::startReceiving -- not connected!") );

  state_ = State_Running;
  receive_thread_ = new boost::thread(&TiDClient::receive, this);
  io_service_thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run,
                                                     &this->io_service_));
  #ifdef WIN32
    SetPriorityClass(receive_thread_->native_handle(),  REALTIME_PRIORITY_CLASS);
    SetThreadPriority(receive_thread_->native_handle(), THREAD_PRIORITY_HIGHEST );
    SetPriorityClass(io_service_thread_->native_handle(),  REALTIME_PRIORITY_CLASS);
    SetThreadPriority(io_service_thread_->native_handle(), THREAD_PRIORITY_HIGHEST );
  #endif
}

//-----------------------------------------------------------------------------

void TiDClient::stopReceiving()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  state_ = State_Stopped;
}

//-----------------------------------------------------------------------------

void TiDClient::receive()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  while(state_ == State_Running)
  {
    try
    {
      msg_parser_->parseMessage(&msg_, input_stream_ );

      #ifdef LPT_TEST
        int port_state = LptPortIn(LPT1,0);
        if(!lpt_flag_)
        {
          lpt_flag_ = 1;
          LptPortOut(LPT1, 0, port_state | 0x02);
        }
        else
        {
          lpt_flag_ = 0;
          LptPortOut(LPT1, 0, port_state & ~0x02);
        }
      #endif

//      #ifdef TIMING_TEST
        double abs_diff = msg_.absolute.Toc();
        double rel_diff = msg_.relative.Toc();
        std::cout << "  ** TIMING TEST ** -- TiDClient::receive() -- abs/rel time-diff:  " << abs_diff <<"/"<< rel_diff;
        std::cout << std::endl << std::flush;
//      #endif

      mutex_.lock();
      messages_.push_back(msg_);
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
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(ec)
    throw(std::runtime_error("TiDClient::handleWrite -- " + ec.message()) );
}

//-----------------------------------------------------------------------------

void TiDClient::getLastMessagesContexts(std::vector< IDMessage >& messages )
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  mutex_.lock();
  messages = messages_;
  messages_.clear();
  mutex_.unlock();
}

//-----------------------------------------------------------------------------

bool TiDClient::newMessagesAvailable()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  bool available = false;
  mutex_.lock();
  available = messages_.size();
  mutex_.unlock();

  return available;
}

//-----------------------------------------------------------------------------

}  //TiD
