#ifndef TIMED_INPUT_STREAM_SOCKET_CPP
#define TIMED_INPUT_STREAM_SOCKET_CPP

#include <boost/chrono.hpp>
#include <string>

#include "libtid/input_stream_socket.h"
#include "libtid/tid_exceptions.h"

namespace TiD
{

class TimedInputStreamSocket : public TiD::InputStreamSocket
{
  public:
    TimedInputStreamSocket(boost::asio::ip::tcp::socket& socket)
      : InputStreamSocket(socket), socket_(socket)
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }

    virtual void readUntil (std::string delimiter, std::string* str)
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      str_buffer_.reserve(4096);
      tmp_str_.reserve(4096);

      if(str_buffer_.size())
      {
        str_buffer_.erase(0, last_pos_+delimiter.size());

        size_t pos = str_buffer_.find(delimiter);
        if(pos != std::string::npos)
        {
          start_time_ = boost::chrono::high_resolution_clock::now();
          *str = str_buffer_.substr(0, pos+delimiter.size() );
          last_pos_ = pos;
          return;
        }
      }

      std::istream is(&stream_buffer_);

      boost::asio::read_until (socket_, stream_buffer_, delimiter,error_ );
      if(error_)
        throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
                                 + std::string (error_.category().name()) + error_.message());

      start_time_ = boost::chrono::high_resolution_clock::now();

      if(str_buffer_.size())
      {
        std::getline(is,tmp_str_);
        str_buffer_.append(tmp_str_);
      }
      else
        std::getline(is,str_buffer_);

      size_t pos = str_buffer_.find(delimiter);
      if(pos != std::string::npos)
      {
        *str = str_buffer_.substr(0, pos+delimiter.size() );
        last_pos_ = pos;
      }
    }

    virtual ~TimedInputStreamSocket()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    };

    boost::chrono::high_resolution_clock::time_point getStartTime()
    {
      return(start_time_);
    }

  private:
    boost::asio::ip::tcp::socket&        socket_;
    boost::system::error_code            error_;

    boost::asio::streambuf               stream_buffer_;
    std::string                          str_buffer_;

    boost::chrono::high_resolution_clock::time_point  start_time_;

};

}

#endif // TIMED_INPUT_STREAM_SOCKET_CPP
