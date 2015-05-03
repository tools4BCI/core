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
        nr_received_msgs_(0), last_frame_nr_(0), data_ready(0)
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      timed_input_stream_ =  new TimedInputStreamSocket(*socket_);
      recv_diffs_.reserve(NR_TID_MESSAGES);
      recv_timepoints_.reserve(NR_TID_MESSAGES);
    }

    //---------------------------------

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

    //---------------------------------

    double sendTimedMessage(std::string& tid_xml_context)
    {
      send_start_time_ = boost::chrono::high_resolution_clock::now();
      sendMessage(tid_xml_context);
      send_diff_ = boost::chrono::high_resolution_clock::now() - send_start_time_;
      return(send_diff_.count());
    }

    //---------------------------------

    double sendTimedMessage(IDMessage& msg)
    {
      send_start_time_ = boost::chrono::high_resolution_clock::now();
      sendMessage(msg);
      send_diff_ = boost::chrono::high_resolution_clock::now() - send_start_time_;
      return(send_diff_.count());
    }

    //---------------------------------

    void sendTimedMessage(IDMessage& msg, boost::chrono::high_resolution_clock::time_point& time_point)
    {
      time_point = boost::chrono::high_resolution_clock::now();
      sendMessage(msg);
    }

    //---------------------------------

    std::vector<double> getRecvDiffValues()
    {
      //      std::cout << "   -->" << BOOST_CURRENT_FUNCTION << " - received msgs: " << nr_received_msgs_;
      //      std::cout << "/" << messages_.size() << std::endl;
      return(recv_diffs_);
    }

    //---------------------------------

    std::vector<boost::chrono::high_resolution_clock::time_point>& getRecvTimePoints()
    {
      return(recv_timepoints_);
    }

    //---------------------------------

    void clearRecvTimingValues()
    {
      timing_mutex_.lock();

      data_ready=false;
      nr_received_msgs_ = 0;

      recv_diffs_.clear();
      recv_timepoints_.clear();
      recv_diffs_.reserve(NR_TID_MESSAGES);
      recv_timepoints_.reserve(NR_TID_MESSAGES);
      timing_mutex_.unlock();
    }

    //---------------------------------

    bool newReceiveDiffsAvailable()
    {
      bool available = false;
      timing_mutex_.lock();
      available = recv_diffs_.size();
      timing_mutex_.unlock();

      return available;
    }

    //---------------------------------

    bool newRecvTimePointsAvailable()
    {
      bool available = false;
      timing_mutex_.lock();
      available = recv_timepoints_.size();
      timing_mutex_.unlock();

      return available;
    }

    //---------------------------------

    void waitForSHM()
    {
      boost::mutex mut;
      boost::unique_lock<boost::mutex> lock(timing_mutex_);

      while(!data_ready)
      {
        shm_cond_.wait(lock);
      }
    }

    //---------------------------------

    void startReceiving(bool throw_on_error)
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      stopReceiving();

      throw_on_error_ = throw_on_error;

      if(state_ != State_Connected )
        if( state_ != State_Stopped )
          throw(std::runtime_error("TiDClient::startReceiving -- not connected!") );

      state_ = State_Running;
      receive_thread_ = new boost::thread(&TimedTiDClient::receive, this);

      io_service_thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run,
                                                         this->io_service_));

      io_service_thread_2_ = new boost::thread(boost::bind(&boost::asio::io_service::run,
                                                           this->io_service_));
      //  &this->io_service_));


      #ifdef WIN32
        SetPriorityClass(receive_thread_->native_handle(),  REALTIME_PRIORITY_CLASS);
        SetThreadPriority(receive_thread_->native_handle(), THREAD_PRIORITY_HIGHEST );

        SetPriorityClass(io_service_thread_->native_handle(),  REALTIME_PRIORITY_CLASS);
        SetThreadPriority(io_service_thread_->native_handle(), THREAD_PRIORITY_HIGHEST );
        SetPriorityClass(io_service_thread_2_->native_handle(),  REALTIME_PRIORITY_CLASS);
        SetThreadPriority(io_service_thread_2_->native_handle(), THREAD_PRIORITY_HIGHEST );
      #endif
    }


    //-----------------------------------------------------------------------------

  private:

    virtual void receiveSHMFinalHook()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      recv_stop_time_ = boost::chrono::high_resolution_clock::now();

      boost::lock_guard<boost::mutex> lock(timing_mutex_);
      recv_timepoints_.push_back(recv_stop_time_);
      data_ready=true;

      shm_cond_.notify_all();
    }

    void receive()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      //TimedTiDClient* inst = static_cast<TimedTiDClient*> (instance);

      nr_received_msgs_ = 0;

      while(state_ == State_Running)
      {
        try
        {
          IDMessage msg;
          msg_parser_->parseMessage(&msg, timed_input_stream_ );

          //msg->Dump();

          mutex_net_msgs_.lock();
          messages_from_net_.push_back(msg);
//          nr_received_msgs_++;
//          if( (last_frame_nr_+1) != msg_.GetBlockIdx() )
//            std::cerr << "Lost a TiD msg -- last/current: " << last_frame_nr_ << "/" << msg_.GetBlockIdx() << std::endl;

//          last_frame_nr_ = msg_.GetBlockIdx();
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

    boost::condition_variable                              shm_cond_;
    bool data_ready;

};

} // TiD

#endif // TID_CLIENT_TIMING_TEST_H

