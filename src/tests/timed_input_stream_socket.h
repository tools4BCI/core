#ifndef TIMED_INPUT_STREAM_SOCKET_CPP
#define TIMED_INPUT_STREAM_SOCKET_CPP

#include <boost/chrono.hpp>
#include <string>

#include "libtid/input_stream_socket.h"
#include "libtid/tid_exceptions.h"

class TimedInputStreamSocket : public TiD::InputStreamSocket
{
  public:
    TimedInputStreamSocket(boost::asio::ip::tcp::socket& socket)
      : InputStreamSocket(socket), socket_(socket)
    {

    }

    virtual void readUntil (std::string delimiter, std::string& str)
    {
      boost::asio::read_until (socket_, stream_buffer_, delimiter,error_ );
      start_time_ = boost::chrono::high_resolution_clock::now();

      if(error_)
      {
        str_buffer_.clear();
        throw TiD::TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
                                      + std::string (error_.category().name()) + error_.message());
      }
      std::istream is(&stream_buffer_);
      std::getline(is, str);
      is.get();

    }

    virtual ~TimedInputStreamSocket() {};

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


#endif // TIMED_INPUT_STREAM_SOCKET_CPP
