/*
    This file is part of the libTiD test routine.

    The libTiD test routine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The libTiD test routine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the libTiD test routine. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#ifndef TIMED_INPUT_STREAM_SOCKET_CPP
#define TIMED_INPUT_STREAM_SOCKET_CPP

#include <boost/chrono.hpp>
#include <string>
#include <thread>

#include "libtid/input_stream_socket.h"
#include "libtid/tid_exceptions.h"
#include "libtid/messages/tid_message_tags_1_0.h"

namespace TiD
{

class TimedInputStreamSocket : public InputStreamSocket
{
  public:
    TimedInputStreamSocket(boost::asio::ip::tcp::socket& socket)
      : InputStreamSocket(socket)
    {
      #ifdef DEBUG
        std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }

    virtual void readUntil (const std::string& delimiter, std::string* str)
    {
      #ifdef DEBUG
        std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      str_buffer_->reserve(4096);
      tmp_str_->reserve(4096);

      if(str_buffer_->size())
      {
        size_t pos = str_buffer_->find(delimiter);
        if(pos != std::string::npos)
        {
          start_time_ = boost::chrono::high_resolution_clock::now();
          *str = str_buffer_->substr(0, pos+delimiter.size() );
          str_buffer_->erase(0, pos+delimiter.size());
          return;
        }
      }

      std::istream is(&stream_buffer_);

      boost::asio::read_until (socket_, stream_buffer_, delimiter, error_ );
      if(error_)
        throw TiDLostConnection ("InputStreamSocket::readUntil error read_until: "
                                 + std::string (error_.category().name()) + error_.message());

      start_time_ = boost::chrono::high_resolution_clock::now();

      if(str_buffer_->size())
      {
        std::getline(is, *tmp_str_);
        str_buffer_->append( *tmp_str_);
      }
      else
        std::getline(is, *str_buffer_);

      size_t pos = str_buffer_->find(delimiter);
      if(pos != std::string::npos)
      {
        *str = str_buffer_->substr(0, pos+delimiter.size() );
        str_buffer_->erase(0, pos+delimiter.size());
      }
    }

    virtual ~TimedInputStreamSocket()
    {
      #ifdef DEBUG
        std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    };

    boost::chrono::high_resolution_clock::time_point getStartTime()
    {
      return(start_time_);
    }

  private:
    boost::chrono::high_resolution_clock::time_point  start_time_;

};

}

#endif // TIMED_INPUT_STREAM_SOCKET_CPP
