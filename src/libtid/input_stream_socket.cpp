#include "input_stream_socket.h"

#include <boost/bind.hpp>
#include <boost/current_function.hpp>

using std::string;

namespace TiD
{

//-----------------------------------------------------------------------------
InputStreamSocket::InputStreamSocket (boost::asio::ip::tcp::socket& socket)
   : socket_ (socket), is_(&stream_buffer_)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  str_buffer_.reserve(2048);
}

//-----------------------------------------------------------------------------
InputStreamSocket::~InputStreamSocket ()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif
}

//-----------------------------------------------------------------------------

void InputStreamSocket::readUntil (char delimiter, std::string& str)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::asio::read_until (socket_, stream_buffer_, delimiter,error_ );

  if(error_)
  {
    str_buffer_.clear();
    throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
                             + string (error_.category().name()) + error_.message());
  }
  std::istream is(&stream_buffer_);
  std::getline(is, str);
  is.get();
}

//-----------------------------------------------------------------------------

void InputStreamSocket::readUntil (std::string delimiter, std::string& str)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  std::istream is(&stream_buffer_);

  if(stream_buffer_.size())
    if(parseIstream(is, str, delimiter) )
      return;

  bool read = 1;
  while(read)
  {
    str_buffer_.clear();
    boost::asio::read_until (socket_, stream_buffer_, delimiter,error_ );

    if(error_)
      throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
                             + string (error_.category().name()) + error_.message());

    read = !parseIstream(is, str_buffer_, delimiter);
    str += str_buffer_;
  }
}

//-----------------------------------------------------------------------------

string InputStreamSocket::readString (unsigned length)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
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
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  return( readString(1).c_str ()[0] );
}

//-----------------------------------------------------------------------------

bool InputStreamSocket::parseIstream(std::istream& stream, std::string& out_string, std::string& delimiter)
{

  if(stream.eof())
    return 0;

  std::getline(stream, out_string, delimiter[0]);
  if(stream.eof())
    return 0;
  out_string.push_back(delimiter[0]);

  char c = 0;
  for(unsigned int n = 1; n < delimiter.size(); )
  {
    stream.get(c);
    out_string.push_back(c);
    if(stream.eof())
      return 0;
    if(c == delimiter[n])
      n++;
    else
    {
      std::getline(stream, out_string, delimiter[0]);
      out_string.push_back(delimiter[0]);
      if(stream.eof())
        return 0;
      n = 1;
    }
  }

   return 1;
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

