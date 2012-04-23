#ifndef TIMED_TID_CLIENT_H
#define TIMED_TID_CLIENT_H

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
        recv_start_time_(boost::chrono::high_resolution_clock::now()),
        nr_received_msgs_(0), last_frame_nr_(0)
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      timed_input_stream_ =  new TimedInputStreamSocket(socket_);
      recv_diffs_.reserve(NR_TID_MESSAGES);
      recv_timepoints_.reserve(NR_TID_MESSAGES);
    }

    virtual ~TimedTiDClient()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

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

    std::vector<double> getRecvDiffValues()
    {
      //      std::cout << "   -->" << BOOST_CURRENT_FUNCTION << " - received msgs: " << nr_received_msgs_;
      //      std::cout << "/" << messages_.size() << std::endl;
      return(recv_diffs_);
    }

    std::vector<boost::chrono::high_resolution_clock::time_point>& getRecvTimePoints()
    {
      return(recv_timepoints_);
    }

    void clearRecvTimingValues()
    {
      timing_mutex_.lock();

      nr_received_msgs_ = 0;

      recv_diffs_.clear();
      recv_timepoints_.clear();
      recv_diffs_.reserve(NR_TID_MESSAGES);
      recv_timepoints_.reserve(NR_TID_MESSAGES);
      timing_mutex_.unlock();
    }

    bool newReceiveDiffsAvailable()
    {
      bool available = false;
      timing_mutex_.lock();
      available = recv_diffs_.size();
      timing_mutex_.unlock();

      return available;
    }

    //-----------------------------------------------------------------------------

  private:
    virtual void receive()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      nr_received_msgs_ = 0;

      while(state_ == State_Running)
      {
        try
        {
          msg_parser_->parseMessage(&msg_, timed_input_stream_ );

          mutex_.lock();
          messages_.push_back(msg_);
//          nr_received_msgs_++;
//          if( (last_frame_nr_+1) != msg_.GetBlockIdx() )
//            std::cerr << "Lost a TiD msg -- last/current: " << last_frame_nr_ << "/" << msg_.GetBlockIdx() << std::endl;

//          last_frame_nr_ = msg_.GetBlockIdx();
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

        timing_mutex_.lock();

        recv_diffs_.push_back(recv_diff_.count());
        recv_timepoints_.push_back(recv_stop_time_);
        timing_mutex_.unlock();

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
    boost::mutex                                           timing_mutex_;

    unsigned int                                           nr_received_msgs_;
    unsigned int                                           last_frame_nr_;

};

} // TiD

#endif // TID_CLIENT_TIMING_TEST_H

