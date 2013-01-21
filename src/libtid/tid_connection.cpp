
#include "tid_connection.h"
#include "tid_server.h"
#include "input_stream_socket.h"
#include "messages/tid_message_parser_1_0.h"
#include "messages/tid_message_builder_1_0.h"

#include <iostream>
#include <boost/current_function.hpp>
#include <boost/asio/error.hpp>

using std::cerr;
using std::cout;
using std::endl;

namespace TiD
{

//-----------------------------------------------------------------------------

TiDConnection::TiDConnection(const TCPConnection::pointer& tcp_conn_handle,
      deleteConnectionCallback del_con_cb, dispatchTiDMessageCallback disp_msg_cb)
 : tcp_connection_(tcp_conn_handle),
   state_(State_Connected), del_callback_ref_(del_con_cb),
   disp_tid_msg_callback_ref_(disp_msg_cb),
   input_stream_(0), msg_parser_(0), msg_builder_(0), receive_thread_(0),
   msg_string_send_buffer_(128)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  //message_buffer_.reserve(TID_MESSAGE_BUFFER_SIZE__IN_BYTE);
  std:: string ip = tcp_conn_handle->socket().remote_endpoint().address().to_string();
  int port = tcp_conn_handle->socket().remote_endpoint().port();
  connection_id_ = make_pair(port, ip);

  input_stream_ = new InputStreamSocket(tcp_conn_handle->socket());
  msg_parser_   = new TiDMessageParser10();
  msg_builder_  = new TiDMessageBuilder10();

  msg_string_send_buffer_.set_capacity(2048);
  current_xml_str_.reserve(2048);
}

//-----------------------------------------------------------------------------

TiDConnection::~TiDConnection()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

//    std::cout << "   -->  " << BOOST_CURRENT_FUNCTION << " -- sent msgs: " << nr_sent_msgs_;
//    std::cout << "   ***  " << " msg buffer size: " << msg_string_send_buffer_.size() << std::endl;

    stop();

  if(state_ != State_ConnectionClosed)
    close();

  if(msg_parser_)
  {
    delete msg_parser_;
    msg_parser_ = 0;
  }

  if(msg_builder_)
  {
    delete msg_builder_;
    msg_builder_ = 0;
  }

  if(receive_thread_)
  {
    receive_thread_->interrupt();
    // receive_thread_->join();  // join causes problems if clients aborted the connection
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

void TiDConnection::run()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  state_ = State_Running;
  receive_thread_ = new boost::thread(&TiDConnection::receive, this );
  #ifdef WIN32
    SetPriorityClass(receive_thread_->native_handle(),  REALTIME_PRIORITY_CLASS);
    SetThreadPriority(receive_thread_->native_handle(), THREAD_PRIORITY_HIGHEST );
  #endif
}

//-----------------------------------------------------------------------------

void TiDConnection::stop()
{
  //#ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  //#endif

  if(state_ != State_Running || state_ != State_ConnectionClosed)
    return;

  state_ = State_Stopped;
  receive_thread_->interrupt();

  boost::system::error_code error;
  tcp_connection_->socket().cancel(error);
  error.clear();

  tcp_connection_->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);

  //receive_thread_->join();

  if(error == boost::asio::error::bad_descriptor)
    return;
  if(error == boost::asio::error::not_connected)
    return;
  else if(error)
    cerr << BOOST_CURRENT_FUNCTION << " -- " << error.message() << endl;
}

//-----------------------------------------------------------------------------

void TiDConnection::close()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(state_ == State_ConnectionClosed)
    return;

  state_ = State_ConnectionClosed;

  boost::system::error_code error;  // ignored error
  if(tcp_connection_->socket().is_open())
    tcp_connection_->socket().close(error);

  //  if(error)
  //    cerr << "TiDConnection::close() -- " << error.message() << endl;

  del_callback_ref_(connection_id_);
}

//-----------------------------------------------------------------------------

void TiDConnection::receive()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  while(state_ == State_Running)
  {
    try
    {
      msg_parser_->parseMessage(&msg_, input_stream_ );
      disp_tid_msg_callback_ref_( msg_ , connection_id_);
    }
    catch(TiDLostConnection&)
    {
      close();
      break;
    }
    catch(TiDException& e)
    {
      if(state_ == State_Running)
        cerr << "TiDException caught -- " << e.what() << endl << ">> ";
      state_ = State_Aborted;
      break;
    }
    catch(TCException& e)
    {
      if(state_ == State_Running)
        cerr << "TCException caught -- " << e.GetCaller() << "" << e.GetInfo() << endl << ">> ";
      state_ = State_Aborted;
      close( );
      break;
    }
    catch(std::exception& e)
    {
      cerr <<BOOST_CURRENT_FUNCTION <<  " -- Caught STL exception while receiving: " << e.what() << endl;
      state_ = State_Aborted;
      close( );
      break;
      //throw;
    }
    catch(...)
    {
      cerr <<BOOST_CURRENT_FUNCTION <<  " -- Caught unknown exception while receiving!" << endl;
      throw;
    }
  }
}

//-----------------------------------------------------------------------------

void TiDConnection::handleWrite(const boost::system::error_code& error,
                                  std::size_t bytes_transferred)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<"--"<< connection_id_.second << ":" <<connection_id_.first<< std::endl;
  #endif

  if (error && (state_ == State_Running) )
  {
    cerr << BOOST_CURRENT_FUNCTION << " [Peer@" << connection_id_.second <<":"<< connection_id_.first;
    try {
      cerr << " -- self: " << tcp_connection_->socket().local_endpoint().address().to_string();
      cerr << ":" << tcp_connection_->socket().local_endpoint().port() << "]: ";
    }
    catch( ... ) { }
    cerr << "-- error-msg: " << error.message() << " --> closing connection." << endl;
    stop();

    // No new asynchronous operations are started. This means that all shared_ptr
    // references to the connection object will disappear and the object will be
    // destroyed automatically after this handler returns. The connection class's
    // destructor closes the socket.
  }
  //else if(state_ == State_Running && (!msg_string_send_buffer_.empty()) )
  //{
  //  msg_string_send_buffer_.pop_front();
  //}
  //if(!msg_string_send_buffer_.empty())
  //  std::cerr << "Still msgs in the buffer -- nr buffered msgs: " << msg_string_send_buffer_.size()  << std::endl;

}

//-----------------------------------------------------------------------------

void TiDConnection::sendMsg(IDMessage& msg)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(msg_string_send_buffer_.full())
  {
    cerr << "TiDConnection::sendMsg [Client@" << connection_id_.second << "]: "
         << "Performance warning -- TiD biffer size too small --> resizing now!" << endl;
    msg_string_send_buffer_.resize( msg_string_send_buffer_.size() *2 );
  }

  boost::system::error_code error;
  msg_builder_->buildTiDMessage(msg, current_xml_str_);
  //msg_string_send_buffer_.push_back( current_xml_str_ );

  boost::asio::write(tcp_connection_->socket(), boost::asio::buffer( current_xml_str_ ), error);

  if (error && (state_ == State_Running) )
  {
    cerr << BOOST_CURRENT_FUNCTION << " [Peer@" << connection_id_.second <<":"<< connection_id_.first;
    try {
      cerr << " -- self: " << tcp_connection_->socket().local_endpoint().address().to_string();
      cerr << ":" << tcp_connection_->socket().local_endpoint().port() << "]: ";
    }
    catch( ... ) { }
    cerr << "-- error-msg: " << error.message() << " --> closing connection." << endl;
    stop();
  }


  #ifndef WIN32
    int i = 1;
    setsockopt( tcp_connection_->socket().native_handle(), IPPROTO_TCP,
                TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif
}

//-----------------------------------------------------------------------------

void TiDConnection::sendMsg(const std::string xml_string)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::system::error_code error;
  boost::asio::write(tcp_connection_->socket(), boost::asio::buffer( xml_string ), error );


  if (error && (state_ == State_Running) )
  {
    cerr << BOOST_CURRENT_FUNCTION << " [Peer@" << connection_id_.second <<":"<< connection_id_.first;
    try {
      cerr << " -- self: " << tcp_connection_->socket().local_endpoint().address().to_string();
      cerr << ":" << tcp_connection_->socket().local_endpoint().port() << "]: ";
    }
    catch( ... ) { }
    cerr << "-- error-msg: " << error.message() << " --> closing connection." << endl;
    stop();
  }

  #ifndef WIN32
    int i = 1;
    setsockopt( tcp_connection_->socket().native_handle(), IPPROTO_TCP,
                TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif

}

//-----------------------------------------------------------------------------

void TiDConnection::asyncSendMsg(IDMessage& msg)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(msg_string_send_buffer_.full())
  {
    cerr << BOOST_CURRENT_FUNCTION << " [Client@" << connection_id_.second << "]: "
         << "Performance warning -- TiD biffer size too small --> resizing now!" << endl;
    msg_string_send_buffer_.resize( msg_string_send_buffer_.size() *2 );
  }

  msg_builder_->buildTiDMessage(msg, current_xml_str_);
  //msg_string_send_buffer_.push_back( current_xml_str_ );

  boost::asio::async_write(tcp_connection_->socket(),
                           boost::asio::buffer( current_xml_str_ ),
                           boost::bind(&TiDConnection::handleWrite, this->shared_from_this(),
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred) );

  #ifndef WIN32
    int i = 1;
    setsockopt( tcp_connection_->socket().native_handle(), IPPROTO_TCP,
                TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif
}

//-----------------------------------------------------------------------------

void TiDConnection::asyncSendMsg(const std::string xml_string)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::asio::async_write(tcp_connection_->socket(),
                           boost::asio::buffer( xml_string ),
                           boost::bind(&TiDConnection::handleWrite, this->shared_from_this(),
                                       boost::asio::placeholders::error,
                                       boost::asio::placeholders::bytes_transferred) );

  #ifndef WIN32
    int i = 1;
    setsockopt( tcp_connection_->socket().native_handle(), IPPROTO_TCP,
                TCP_QUICKACK, (void *)&i, sizeof(i));
  #endif

}

//-----------------------------------------------------------------------------

}  //TiD
