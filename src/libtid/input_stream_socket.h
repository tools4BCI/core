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
  public:
    InputStreamSocket (boost::asio::ip::tcp::socket& socket);
    virtual ~InputStreamSocket ();
    virtual std::string readUntil (char delimiter);
    virtual std::string readUntil (std::string delimiter);
    virtual std::string readString (unsigned int length);
    virtual char readCharacter ();

//  private:
//    void readLineHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);
//    void readStringHandler(const boost::system::error_code& ec, std::size_t bytes_transfered);


  private:
    boost::asio::ip::tcp::socket&        socket_;

    boost::asio::streambuf               stream_buffer_;
    std::string                          str_buffer_;
};

}

#endif // INPUT_STREAM_SOCKET_H
