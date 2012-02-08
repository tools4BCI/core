#include "input_stream_socket.h"

#include <boost/bind.hpp>

using std::string;

namespace TiD
{

//-----------------------------------------------------------------------------
InputStreamSocket::InputStreamSocket (boost::asio::ip::tcp::socket& socket)
   : socket_ (socket)
{
  #ifdef DEBUG
    std::cout << "InputStreamSocket: Constructor" << std::endl;
  #endif
}

//-----------------------------------------------------------------------------
InputStreamSocket::~InputStreamSocket ()
{
  #ifdef DEBUG
    std::cout << "InputStreamSocket: ~InputStreamSocket" << std::endl;
  #endif
}

//-----------------------------------------------------------------------------

string InputStreamSocket::readUntil (char delimiter)
{
  #ifdef DEBUG
    std::cout << "InputStreamSocket: readUntil(char delimiter)" << std::endl;
  #endif

  boost::system::error_code  error;

  boost::asio::read_until (socket_, stream_buffer_, delimiter,error );

  if(error)
    throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
                             + string (error.category().name()) + error.message());
  str_buffer_.clear();
  std::istream is(&stream_buffer_);
  std::getline(is, str_buffer_);
  is.get();

  return(str_buffer_);
}

//-----------------------------------------------------------------------------

string InputStreamSocket::readUntil (std::string delimiter)
{
  #ifdef DEBUG
    std::cout << "InputStreamSocket: readUntil(std::string delimiter)" << std::endl;
  #endif

  boost::system::error_code  error;

  boost::asio::read_until (socket_, stream_buffer_, delimiter,error );

  if(error)
    throw TiDLostConnection ("InputStreamSocket::readLine error read_until: "
                             + string (error.category().name()) + error.message());
  str_buffer_.clear();
  std::istream is(&stream_buffer_);
  std::getline(is, str_buffer_);
  is.get();

  return(str_buffer_);
}

//-----------------------------------------------------------------------------

string InputStreamSocket::readString (unsigned length)
{
  #ifdef DEBUG
    std::cout << "InputStreamSocket: readString" << std::endl;
  #endif

  if(stream_buffer_.size() < length)
  {
    boost::system::error_code  error;
    boost::asio::read (socket_, stream_buffer_,
                       boost::asio::transfer_at_least( length - stream_buffer_.size() ), error ) ;
    if (error)
      throw TiDLostConnection ("InputStreamSocket::readStringHandler error read: "
                               + string (error.category().name()) + error.message());
  }

  str_buffer_.clear();
  std::istream is(&stream_buffer_);
  str_buffer_.resize(length);

  for(unsigned int n = 0; n < length; n++)
  {
    str_buffer_[n] = is.peek();
    stream_buffer_.consume(1);
  }
  return(str_buffer_);
}

//-----------------------------------------------------------------------------

char InputStreamSocket::readCharacter ()
{
  #ifdef DEBUG
    std::cout << "InputStreamSocket: readCharacter" << std::endl;
  #endif

  return( readString(1).c_str ()[0] );
}

//-----------------------------------------------------------------------------

}


//void InputStreamSocket::readStringHandler(const boost::system::error_code& ec,
//                                          std::size_t /*bytes_transfered*/)
//{
//  if (ec)
//  {
//    error_ = ec;
//    cond_.notify_one();
//    return;
//  }


//  str_buffer_.clear();
//  std::istream is(&stream_buffer_);
//  std::getline(is, str_buffer_);

//  cond_.notify_one();
//}


//-----------------------------------------------------------------------------

//void InputStreamSocket::readLineHandler(const boost::system::error_code& ec,
//                                        std::size_t /*bytes_transfered*/)
//{
//  if (ec)
//  {
//    error_ = ec;
//    cond_.notify_one();
//    return;
//  }

//  str_buffer_.clear();
//  std::istream is(&stream_buffer_);
//  std::getline(is, str_buffer_);

//  cond_.notify_one();
//}

