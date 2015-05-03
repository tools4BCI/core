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

#ifndef LPT_TID_CLIENT
#define LPT_TID_CLIENT

#include "libtid/tid_client.h"
#include "libtid/messages/tid_message_parser_1_0.h"

extern unsigned int NR_TID_MESSAGES;

namespace TiD
{

class LPTTiDClient : public TiD::TiDClient
{
  public:
    LPTTiDClient()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }

    virtual ~LPTTiDClient()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif
    }

    void sendLPTMessage(std::string& tid_xml_context)
    {
      sendMessage(tid_xml_context);
    }

    void sendLPTMessage(IDMessage& msg)
    {
      sendMessage(msg);
    }

    //-----------------------------------------------------------------------------

  private:
    virtual void receive()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      while(state_ == State_Running)
      {
        try
        {
          IDMessage msg;
          msg_parser_->parseMessage(&msg, input_stream_ );
          mutex_net_msgs_.lock();
          messages_from_net_.push_back(msg);
          mutex_net_msgs_.unlock();
        }
        catch(std::exception& e)
        {
          if(throw_on_error_)
            boost::throw_exception(e);

          if(state_ == State_Running)
            std::cerr << e.what() << std::endl << ">> ";
          state_ = State_Error;
          return;
        }
      }
    }



  private:

};

} // TiD

#endif // TID_CLIENT_TIMING_TEST_H

