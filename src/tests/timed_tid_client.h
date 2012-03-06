#ifndef TID_CLIENT_TIMING_TEST_H
#define TID_CLIENT_TIMING_TEST_H

#include <boost/chrono.hpp>

#include "libtid/tid_client.h"
#include "timed_input_stream_socket.h"
#include "libtid/messages/tid_message_parser_1_0.h"


extern unsigned int NR_TID_MESSAGES;

namespace TiD
{

class TimedTiDClient : public TiD::TiDClient
{
  public:
    TimedTiDClient()
      : send_start_time_(boost::chrono::high_resolution_clock::now()),
        recv_start_time_(boost::chrono::high_resolution_clock::now())
    {
      timed_input_stream_ =  new TimedInputStreamSocket(socket_);
      recv_diffs_.reserve(NR_TID_MESSAGES);
      recv_timepoints_.reserve(NR_TID_MESSAGES);
    }

    virtual ~TimedTiDClient()
    {
      if(timed_input_stream_)
      {
        delete timed_input_stream_;
        timed_input_stream_ = 0;
      }
    }

    double sendTimedMessage(std::string& tid_xml_context)
    {
      send_start_time_ = boost::chrono::high_resolution_clock::now();
      sendMessage(tid_xml_context);
      send_diff_ = boost::chrono::high_resolution_clock::now() - send_start_time_;
      return(send_diff_.count());
    }

    double sendTimedMessage(IDMessage& msg)
    {
      send_start_time_ = boost::chrono::high_resolution_clock::now();
      sendMessage(msg);
      send_diff_ = boost::chrono::high_resolution_clock::now() - send_start_time_;
      return(send_diff_.count());
    }

    void sendTimedMessage(IDMessage& msg, boost::chrono::high_resolution_clock::time_point& time_point)
    {
      time_point = boost::chrono::high_resolution_clock::now();
      sendMessage(msg);
    }

    std::vector<double>& getRecvDiffValues()
    {
      return(recv_diffs_);
    }

    std::vector<boost::chrono::high_resolution_clock::time_point>& getRecvTimePoints()
    {
      return(recv_timepoints_);
    }

    void clearRecvTimingValues()
    {
      recv_diffs_.clear();
      recv_timepoints_.clear();
    }

    //-----------------------------------------------------------------------------

  private:
    virtual void receive()
    {
      //std::cout << "    " << BOOST_CURRENT_FUNCTION <<  std::endl;

      while(state_ == State_Running)
      {
        try
        {
          msg_parser_->parseMessage(&msg_, timed_input_stream_ );

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
        recv_stop_time_ = boost::chrono::high_resolution_clock::now();
        recv_start_time_ = timed_input_stream_->getStartTime();
        recv_diff_ = recv_stop_time_ - recv_start_time_;

        recv_diffs_.push_back(recv_diff_.count());
        recv_timepoints_.push_back(recv_stop_time_);

        //std::cout << recv_diff_ << " -- " << recv_diffs_.size() << std::endl;
      }
    }



  private:
    boost::chrono::high_resolution_clock::time_point      send_start_time_;
    boost::chrono::high_resolution_clock::time_point      send_stop_time_;
    boost::chrono::duration<double, boost::micro>         send_diff_;

    boost::chrono::high_resolution_clock::time_point      recv_start_time_;
    boost::chrono::high_resolution_clock::time_point      recv_stop_time_;
    boost::chrono::duration<double, boost::micro>         recv_diff_;

    std::vector<double>                                   recv_diffs_;

    std::vector<boost::chrono::high_resolution_clock::time_point>    recv_timepoints_;

    TimedInputStreamSocket*                                timed_input_stream_;
};

} // TiD

#endif // TID_CLIENT_TIMING_TEST_H

