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

    double sendLPTMessage(std::string& tid_xml_context)
    {

      sendMessage(tid_xml_context);
    }

    double sendLPTMessage(IDMessage& msg)
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
          msg_parser_->parseMessage(&msg_, input_stream_ );
          mutex_.lock();
          messages_.push_back(msg_);
          mutex_.unlock();
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

