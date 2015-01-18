#ifndef INPUT_STREAM_SOCKET_H
#define INPUT_STREAM_SOCKET_H

#include "input_stream.h"
#include "tid_exceptions.h"

#include <boost/asio.hpp>
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

    virtual void readUntil (const std::string& delimiter, std::string* str);

//    virtual void readUntil (std::string* str);
//    virtual void setDelimiter(const std::string& del);

    virtual void fustyreadUntil (const std::string& delimiter, std::string* str);

  private:
    bool parseIstream(std::istream& stream, std::string* out_string, const std::string& delimiter);

  //    void readLineHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);
  //    void readStringHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);


  private:
    std::string*                         str_buffer_;
    std::string*                         tmp_str_;

    boost::asio::ip::tcp::socket&        socket_;
    boost::system::error_code            error_;


    // just used within the fusty version -- will be removed in future
    boost::asio::streambuf               stream_buffer_;


    size_t                               last_pos_;
//    std::string                          delimiter_;
};

}

#endif // INPUT_STREAM_SOCKET_H
