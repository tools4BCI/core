
#include "tid_connection.h"
#include "tid_server.h"
#include "input_stream_socket.h"
#include "messages/tid_message_parser_1_0.h"
#include "messages/tid_message_builder_1_0.h"

#include <iostream>

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
   input_stream_(0), msg_parser_(0), msg_builder_(0), receive_thread_(0)
{
  #ifdef DEBUG
    std::cout << "TiDConnection: Constructor" << std::endl;
  #endif

  message_buffer_.reserve(TID_MESSAGE_BUFFER_SIZE__IN_BYTE);
  std:: string ip = tcp_conn_handle->socket().remote_endpoint().address().to_string();
  int port = tcp_conn_handle->socket().remote_endpoint().port();
  connection_id_ = make_pair(port, ip);

  input_stream_ = new InputStreamSocket(tcp_conn_handle->socket());
  msg_parser_   = new TiDMessageParser10();
  msg_builder_  = new TiDMessageBuilder10();
}

//-----------------------------------------------------------------------------

TiDConnection::~TiDConnection()
{
  #ifdef DEBUG
    std::cout << "TiDConnection: ~TiDConnection" << std::endl;
  #endif

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
    //    receive_thread_->interrupt();
    //    receive_thread_->join();
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
    std::cout << "TiDConnection: run" << std::endl;
  #endif

  state_ = State_Running;
  receive_thread_ = new boost::thread(&TiDConnection::receive, this );
}

//-----------------------------------------------------------------------------

void TiDConnection::stop()
{
  #ifdef DEBUG
    std::cout << "TiDConnection: stop" << std::endl;
  #endif

  state_ = State_Stopped;
}

//-----------------------------------------------------------------------------

void TiDConnection::close()
{
  #ifdef DEBUG
    std::cout << "TiDConnection::close [Client @" << connection_id_.second << "]" << std::endl;
  #endif

  if(state_ == State_ConnectionClosed)
    return;

  boost::system::error_code error;  // ignored error

  tcp_connection_->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
  tcp_connection_->socket().close(error);
  state_ = State_ConnectionClosed;

  del_callback_ref_(connection_id_);
}

//-----------------------------------------------------------------------------

void TiDConnection::receive()
{
  #ifdef DEBUG
    std::cout << "TiDConnection: receive" << std::endl;
  #endif

  while(state_ == State_Running)
  {
    try
    {
      disp_tid_msg_callback_ref_( msg_parser_->parseMessage( *input_stream_ ) , connection_id_);
    }
    catch(TiDLostConnection&)
    {
      close();
      break;
    }
    catch(TiDException& e)
    {
      if(state_ == State_Running)
        cerr << e.what() << endl << ">> ";
      close();
      break;
    }
    catch(...)
    {
      cerr << "Caught unknown exception during message parsing!" << endl;
      throw;
    }
  }
}

//-----------------------------------------------------------------------------

void TiDConnection::handleWrite(const boost::system::error_code& error,
                                  std::size_t bytes_transferred)
{
  #ifdef DEBUG
    std::cout << "TiDConnection: handleWrite" << std::endl;
  #endif

  if (error)
  {
    cerr << "TiDConnection::handleWrite [Client@" << connection_id_.second << "]: "
         << "error during write - closing connection."
         << "--> " << error.message() << endl;
    close();

    // No new asynchronous operations are started. This means that all shared_ptr
    // references to the connection object will disappear and the object will be
    // destroyed automatically after this handler returns. The connection class's
    // destructor closes the socket.
    return;
  }

}
//-----------------------------------------------------------------------------

void TiDConnection::sendMsg(IDMessage& msg)
{
  #ifdef DEBUG
    std::cout << "TiDConnection: sendMsg" << std::endl;
  #endif

  std::string str( msg_builder_->buildTiDMessage(msg) );
  boost::asio::async_write(tcp_connection_->socket(), boost::asio::buffer(str),
                           boost::bind(&TiDConnection::handleWrite, this,
                                       boost::asio::placeholders::error, str.size()) );
}

//-----------------------------------------------------------------------------

}  //TiD
