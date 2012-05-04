#include "input_stream_socket.h"

#include <boost/bind.hpp>
#include <boost/current_function.hpp>

using std::string;

namespace TiD
{

//-----------------------------------------------------------------------------
InputStreamSocket::InputStreamSocket (boost::asio::ip::tcp::socket& socket)
  : socket_ (socket), last_pos_(0)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  str_buffer_ = new std::string;
  tmp_str_ = new std::string;

  str_buffer_->reserve(4096);
  tmp_str_->reserve(4096);

  // just used within the fusty version -- will be removed in future
  stream_buffer_.prepare(8192);
}

//-----------------------------------------------------------------------------
InputStreamSocket::~InputStreamSocket ()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::system::error_code ec;
  socket_.close(ec);

  if(str_buffer_)
    delete str_buffer_;
  if(tmp_str_)
    delete tmp_str_;
}

//-----------------------------------------------------------------------------

//void InputStreamSocket::setDelimiter (const std::string& del)
//{
//  #ifdef DEBUG
//    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
//  #endif
//  delimiter_ = del;

//  std::cout << "delimiter:" << delimiter_ <<  std::endl << std::flush;
//  std::cerr << "str_buffer: " << str_buffer_ << std::endl;
//}

//-----------------------------------------------------------------------------

//void InputStreamSocket::readUntil (std::string* str)
//{
////  #ifdef DEBUG
//    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl << std::flush;
//    std::cout << "delimiter:" << delimiter_ <<  std::endl << std::flush;
////  #endif

//  str_buffer_.reserve(4096);
//  tmp_str_.reserve(4096);

//  std::cout << "delimiter:" << delimiter_ <<  std::endl << std::flush;
//  std::cerr << "str_buffer: " << str_buffer_ << std::endl;

//  if(str_buffer_.size())
//  {
//    size_t pos = str_buffer_.find(delimiter_);
//    if(pos != std::string::npos)
//    {
//      *str = str_buffer_.substr(0, pos+delimiter_.size() );
//      str_buffer_.erase(0, pos+delimiter_.size());
//      return;
//    }
//  }
//  std::istream is(&stream_buffer_);

//  std::cout << "delimiter:" << delimiter_ <<  std::endl << std::flush;
//  std::cerr << "str_buffer: " << str_buffer_ << std::endl;

//  std::string const x("/>");

//  boost::asio::read_until (socket_, stream_buffer_, x, error_ );
//  if(error_)
//    throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
//                             + std::string (error_.category().name()) + error_.message());

//  std::cout << "delimiter:" << delimiter_ <<  std::endl << std::flush;
//  std::cerr << "str_buffer: " << str_buffer_ << std::endl;

//  if(str_buffer_.size())
//  {
//    std::getline(is,tmp_str_);
//    str_buffer_.append(tmp_str_);
//  }
//  else
//    std::getline(is,str_buffer_);

//  size_t pos = str_buffer_.find(delimiter_);
//  if(pos != std::string::npos)
//  {
//    *str = str_buffer_.substr(0, pos+delimiter_.size() );
//    str_buffer_.erase(0, pos+delimiter_.size());
//  }

//  std::cerr << "str: " << *str << std::endl;

//}

//-----------------------------------------------------------------------------

void InputStreamSocket::readUntil (const std::string&  delimiter, std::string* str)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl << std::flush;
  #endif

  str_buffer_->reserve(2048);
  tmp_str_->reserve(2048);


  if(str_buffer_->size())
  {
    size_t pos = str_buffer_->find(delimiter);
    if(pos != std::string::npos)
    {
      *str = str_buffer_->substr(0, pos+delimiter.size() );
      str_buffer_->erase(0, pos+delimiter.size());
      return;
    }
  }
  std::istream is(&stream_buffer_);

  boost::asio::read_until (socket_, stream_buffer_, delimiter,error_ );
  if(error_)
    throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
                             + std::string (error_.category().name()) + " " + error_.message());

  if(str_buffer_->size())
  {
    std::getline(is, *tmp_str_);
    str_buffer_->append(*tmp_str_);
  }
  else
    std::getline(is, *str_buffer_);

  size_t pos = str_buffer_->find(delimiter);
  if(pos != std::string::npos)
  {
    *str = str_buffer_->substr(0, pos+delimiter.size() );
    str_buffer_->erase(0, pos+delimiter.size());
  }

//  while(1)
//  {
//    size_t vals = boost::asio::read( socket_, boost::asio::buffer(buffer_vec_),
//                                     boost::asio::transfer_at_least(80), error_);
//    if(error_)
//      throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
//                               + string (error_.category().name()) + error_.message());

//    for(size_t n = 0; n < vals; n++)
//      str_buffer_.push_back(buffer_vec_[n]);

//    size_t pos = str_buffer_.find(delimiter);
//    if(pos != std::string::npos)
//    {
//      *str = str_buffer_.substr(0, pos+delimiter.size() );
//      str_buffer_.erase(0, pos+delimiter.size());
//      return;
//    }

//  }
}

//-----------------------------------------------------------------------------

void InputStreamSocket::fustyReadUntil (std::string delimiter, std::string* str)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

//    std::istream is(&stream_buffer_);

//  if(stream_buffer_.size())
//  {
////    std::istream is(&stream_buffer_);
//    if(parseIstream(is, str, delimiter) )
//      return;
//  }

//  bool read = 1;
//  while(read)
//  {
//    str_buffer_.clear();
//    boost::asio::read_until (socket_, stream_buffer_, delimiter,error_ );

//    if(error_)
//      throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
//                             + string (error_.category().name()) + error_.message());

////    std::istream is(&stream_buffer_);

//    read = !parseIstream(is, &str_buffer_, delimiter);
//    *str += str_buffer_;
//  }
}

//-----------------------------------------------------------------------------

bool InputStreamSocket::parseIstream(std::istream& stream, std::string* out_string, std::string& delimiter)
{

  if(stream.eof())
    return 0;

  std::getline(stream, *out_string, delimiter[0]);
  if(stream.eof())
    return 0;
  out_string->push_back(delimiter[0]);

  char c = 0;
  for(unsigned int n = 1; n < delimiter.size(); )
  {
    stream.get(c);
    out_string->push_back(c);
    if(stream.eof())
      return 0;
    if(c == delimiter[n])
      n++;
    else
    {
      std::getline(stream, *out_string, delimiter[0]);
      out_string->push_back(delimiter[0]);
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


//string InputStreamSocket::readString (unsigned length)
//{
//  #ifdef DEBUG
//    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
//  #endif

//  if(stream_buffer_.size() < length)
//  {
//    boost::system::error_code  error;
//    boost::asio::read (socket_, stream_buffer_,
//                       boost::asio::transfer_at_least( length - stream_buffer_.size() ), error ) ;
//    if (error)
//      throw TiDLostConnection ("InputStreamSocket::readStringHandler error read: "
//                               + string (error.category().name()) + error.message());
//  }

//  str_buffer_.clear();
//  std::istream is(&stream_buffer_);
//  str_buffer_.resize(length);

//  for(unsigned int n = 0; n < length; n++)
//  {
//    str_buffer_[n] = is.peek();
//    stream_buffer_.consume(1);
//  }
//  return(str_buffer_);
//}

////-----------------------------------------------------------------------------

//char InputStreamSocket::readCharacter ()
//{
//  #ifdef DEBUG
//    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
//  #endif

//  return( readString(1).c_str ()[0] );
//}


