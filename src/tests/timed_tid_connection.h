#ifndef TIMED_TID_CONNECTION_H
#define TIMED_TID_CONNECTION_H

#include <boost/chrono.hpp>

#include "libtid/tcp_connection.h"
#include "libtid/tid_connection.h"
#include "timed_input_stream_socket.h"
#include "libtid/messages/tid_message_parser_1_0.h"

extern unsigned int NR_TID_MESSAGES;

namespace TiD
{

class TimedTiDConnection : public TiDConnection
{
  public:

    typedef boost::shared_ptr<TiDConnection> pointer;

    virtual ~TimedTiDConnection()
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

  static pointer create(const TCPConnection::pointer& tcp_conn_handle,
                        deleteConnectionCallback del_con_cb, dispatchTiDMessageCallback disp_msg_cb)
  {
    return pointer(new TimedTiDConnection(tcp_conn_handle, del_con_cb, disp_msg_cb));
  }

  std::vector<double>& getRecvTimingValues()
  {
    return(recv_diffs_);
  }

  void clearRecvTimingValues()
  {
    recv_diffs_.clear();
  }


  private:
    TimedTiDConnection(const TCPConnection::pointer& tcp_conn_handle,
                       deleteConnectionCallback del_con_cb, dispatchTiDMessageCallback disp_msg_cb)
      : TiDConnection(tcp_conn_handle, del_con_cb, disp_msg_cb),
        recv_start_time_(boost::chrono::high_resolution_clock::now())
    {
      timed_input_stream_ =  new TimedInputStreamSocket(tcp_conn_handle->socket());
      recv_diffs_.reserve(NR_TID_MESSAGES);
    }

    virtual void receive()
    {
      #ifdef DEBUG
        std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
      #endif

      while(state_ == State_Running)
      {
        try
        {
          msg_parser_->parseMessage(&msg_, timed_input_stream_ );
          disp_tid_msg_callback_ref_( msg_ , connection_id_);
        }
        catch(TiDLostConnection&)
        {
          close();
          break;
        }
        catch(TiDException& e)
        {
          if(state_ == State_Running)
            std::cerr << e.what() << std::endl << ">> ";
          close();
          break;
        }
        catch(...)
        {
          std::cerr << "Caught unknown exception during message parsing!" << std::endl;
          throw;
        }
        recv_stop_time_ = boost::chrono::high_resolution_clock::now();
        recv_start_time_ = timed_input_stream_->getStartTime();
        recv_diff_ = recv_stop_time_ - recv_start_time_;
        //        std::cout << recv_diff_ << std::endl;

        recv_diffs_.push_back(recv_diff_.count());
      }
  }

  private:
    boost::chrono::high_resolution_clock::time_point      recv_start_time_;
    boost::chrono::high_resolution_clock::time_point      recv_stop_time_;
    boost::chrono::duration<double, boost::micro>         recv_diff_;

    std::vector<double>                                   recv_diffs_;

    TimedInputStreamSocket*                               timed_input_stream_;

};

}

#endif // TIMED_TID_CONNECTION_H
