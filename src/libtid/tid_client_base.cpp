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

static const int SOCKET_BUFFER_SIZE = 65536;

//-----------------------------------------------------------------------------

TiDClientBase::TiDClientBase()
  : remote_port_(0), state_(State_ConnectionClosed), socket_(io_service_),
    input_stream_(0), msg_parser_(0), msg_builder_(0), throw_on_error_(0)
      //strand_(io_service_)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  input_stream_ = new InputStreamSocket(socket_);
  msg_parser_   = new TiDMessageParser10();
  msg_builder_  = new TiDMessageBuilder10();

  messages_.reserve(100);
  xml_string_.reserve(2048);
}

//-----------------------------------------------------------------------------

TiDClientBase::~TiDClientBase()
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

  if(input_stream_)
  {
    delete input_stream_;
    input_stream_ = 0;
  }
}

//-----------------------------------------------------------------------------

void TiDClientBase::connect(std::string ip_addr, unsigned int port)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::system::error_code ec;

  if(socket_.is_open())
  {
    std::stringstream ex_str;
    ex_str << "TiDClient: Already connected!" << socket_.remote_endpoint().address().to_string();
    throw(std::invalid_argument( ex_str.str() ));
  }

  boost::asio::ip::tcp::endpoint peer(boost::asio::ip::address::from_string(ip_addr),port );

  socket_.connect(peer, ec);
  if(ec)
    throw(std::runtime_error("TiDClient::connect -- " + ec.message()) );

  state_ = State_Connected;
  remote_port_ = port;
  remote_ip_ = ip_addr;

  boost::asio::socket_base::send_buffer_size send_buffer_option(SOCKET_BUFFER_SIZE);
  socket_.set_option(send_buffer_option);
  boost::asio::socket_base::receive_buffer_size recv_buffer_option(SOCKET_BUFFER_SIZE);
  socket_.set_option(recv_buffer_option);
  boost::asio::ip::tcp::no_delay delay(true);
  socket_.set_option(delay);
  boost::asio::socket_base::linger linger(false, 0);
  socket_.set_option(linger);

  #ifndef WIN32
    int i = 1;
    setsockopt( socket_.native_handle(), IPPROTO_TCP,
              TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif
}

//-----------------------------------------------------------------------------

void TiDClientBase::disconnect()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(state_ == State_ConnectionClosed)
    return;

  boost::system::error_code ec;  //ignored

  socket_.cancel(ec);
  unsigned int nr_disconnect_retries = 0;
  ec.clear();

  do
  {
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket_.close(ec);
    if(ec)
    {
      std::cerr << BOOST_CURRENT_FUNCTION << " -- " << ec.message() << std::endl;
      #ifdef WIN32
        Sleep(1);
      #else
        usleep(10000);
      #endif


    }
    nr_disconnect_retries++;
  }
  while(ec && (nr_disconnect_retries < 10) ) ;

  state_ = State_ConnectionClosed;

  //  std::cout << "   -->" << BOOST_CURRENT_FUNCTION << " - sent msgs: " << nr_sent_msgs_;
  //  std::cout << std::endl;
}

//-----------------------------------------------------------------------------

void TiDClientBase::setBufferSize(size_t size)
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

void TiDClientBase::reserveNrOfMsgs (size_t expected_nr_of_msgs)
{
  messages_.reserve(expected_nr_of_msgs);
}

//-----------------------------------------------------------------------------

void TiDClientBase::sendMessage(std::string& tid_xml_context)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(!socket_.is_open())
    return;

  boost::asio::async_write(socket_, boost::asio::buffer(tid_xml_context),
                             boost::bind(&TiDClientBase::handleWrite, this,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred ));

  io_service_.reset();
  io_service_.run_one();
  io_service_.poll_one();

  #ifndef WIN32
    int i = 1;
    setsockopt( socket_.native_handle(), IPPROTO_TCP,
              TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif
}

//-----------------------------------------------------------------------------

void TiDClientBase::sendMessage(IDMessage& msg)
{
  #ifdef DEBUG
    std::cout << "  --> " << BOOST_CURRENT_FUNCTION  <<  std::endl;
  #endif

  if(!socket_.is_open())
    return;

  msg_builder_->buildTiDMessage(msg, xml_string_);

  boost::asio::async_write(socket_, boost::asio::buffer(xml_string_),
                           boost::bind(&TiDClientBase::handleWrite, this,
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred ));
  io_service_.reset();
  io_service_.run_one();
  io_service_.poll_one();

  xml_string_.clear();
  #ifndef WIN32
    int i = 1;
    setsockopt( socket_.native_handle(), IPPROTO_TCP,
              TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif
}

//-----------------------------------------------------------------------------

int TiDClientBase::receive(void* instance)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  TiDClientBase* inst = static_cast<TiDClientBase*> (instance);

  while(inst->state_ == State_Running)
  {
    std::cout << "receiving ..." << std::endl;

    try
    {
      inst->msg_parser_->parseMessage(&(inst->msg_), inst->input_stream_ );
      inst->mutex_.lock();
      inst->messages_.push_back(inst->msg_);
      inst->mutex_.unlock();
    }
    catch(TiDLostConnection&)
    {
//      stopReceiving();
      std::cerr << "   ***  Connection to TiD Server@" <<
        inst->remote_ip_ << ":" << inst->remote_port_ << " lost." << std::endl << " >> ";
      break;
    }
    catch(TiDException& e)
    {
      if(inst->state_ == State_Running)
        std::cerr << e.what() << std::endl << ">> ";
//      stopReceiving();
      break;
    }
    catch(std::exception& e)
    {
      if(inst->throw_on_error_)
        boost::throw_exception(e);

      if(inst->state_ == State_Running)
        std::cerr << e.what() << std::endl << ">> ";
      inst->state_ = State_Error;
      return 1;
    }
  }
  //inst->state_ = State_Error;

  return 0;
}

//-----------------------------------------------------------------------------

void TiDClientBase::handleWrite(const boost::system::error_code &ec,
                              std::size_t bytes_transferred)
{
  #ifdef DEBUG
    std::cerr << BOOST_CURRENT_FUNCTION << " -- " << ec.message() <<  std::endl;
  #endif

  if(ec)
      throw(std::runtime_error("TiDClient::handleWrite -- " + ec.message()
                               + "; transferred: " + boost::lexical_cast<std::string>(bytes_transferred) ) );
}

//-----------------------------------------------------------------------------

//void TiDClient::completionHandler(std::string& msg)
//{
//  std::cerr << BOOST_CURRENT_FUNCTION  <<  " -- "  << msg <<  std::endl << std::flush;

//  boost::asio::async_write(socket_, boost::asio::buffer(msg),
//                           strand_.wrap( boost::bind(&TiDClient::handleWrite, this,
//                                                      boost::asio::placeholders::error,
//                                                      boost::asio::placeholders::bytes_transferred)  ));

////  io_service_.poll_one();

//}

//-----------------------------------------------------------------------------

void TiDClientBase::getLastMessagesContexts(std::vector< IDMessage >& messages )
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

bool TiDClientBase::newMessagesAvailable()
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

void TiDClientBase::clearMessages()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  mutex_.lock();
  messages_.clear();
  mutex_.unlock();
}

//-----------------------------------------------------------------------------

bool TiDClientBase::receiving()
{
  bool val = 0;

  mutex_.lock();
  if(state_ == State_Running)
    val = true;
  mutex_.unlock();

  return val;
}

//-----------------------------------------------------------------------------

}  //TiD
