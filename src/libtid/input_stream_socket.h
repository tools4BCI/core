#ifndef INPUT_STREAM_SOCKET_H
#define INPUT_STREAM_SOCKET_H

#include "input_stream.h"
#include "tid_exceptions.h"

#include <boost/asio.hpp>
#include <boost/thread/condition_variable.hpp>

#include <string>

namespace TiD
{

//-----------------------------------------------------------------------------
/// base class for any socket
class InputStreamSocket : public InputStream
{
  friend class TimedInputStreamSocket;

  public:
    InputStreamSocket (boost::asio::ip::tcp::socket& socket);
    virtual ~InputStreamSocket ();

    virtual void readUntil (std::string delimiter, std::string* str);
    virtual void fustyReadUntil (std::string delimiter, std::string* str);

  private:
    bool parseIstream(std::istream& stream, std::string* out_string, std::string& delimiter);

  //    void readLineHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);
  //    void readStringHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);


  private:
    boost::asio::ip::tcp::socket&        socket_;
    boost::system::error_code            error_;

    std::string                          str_buffer_;

    // just used within the fusty version -- will be removed in future
    boost::asio::streambuf               stream_buffer_;
    std::string                          tmp_str_;

    size_t                               last_pos_;
};

}

#endif // INPUT_STREAM_SOCKET_H
