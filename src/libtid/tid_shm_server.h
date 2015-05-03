/*
    This file is part of TOBI Interface D (TiD).

    TOBI Interface D (TiD) is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TOBI Interface D (TiD) is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TOBI Interface D (TiD).  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#ifndef TID_SHM_SERVER_H
#define TID_SHM_SERVER_H

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>


//-----------------------------------------------------------------------------

/**
* @class TiDSHMServer
*
* @brief TODO
*/

class IDMessage;

namespace TiD
{

typedef boost::function< void( IDMessage&, const std::pair<int, std::string>& ) > dispatchTiDMessageCallback;

class TiDSHMServer
{
  public:
    TiDSHMServer(dispatchTiDMessageCallback disp_tid_msg_callback_ref);
    virtual ~TiDSHMServer();


    void setMaxMsgSize(unsigned int size);
    void setMaxNrOfMsgs(unsigned int nr_msgs);

    void addSHMMessageQueue(const std::string& name);
    void closeMsgQueue(const std::string& name);
    void closeAllMsgQueues();

    void startMessageQueueProcessing(const std::string& name);
    void stopMessageQueueProcessing(const std::string& name);
    void stopMessageQueueProcessing();



  private:
    typedef std::map< std::string, std::pair< std::pair<boost::interprocess::message_queue*, boost::interprocess::message_queue*>, boost::thread*> > QueueMap;
    void receive(QueueMap::iterator it);

    void dispatchMsgToOtherQueues(std::string& msg, const std::string& origin);


  private:
    QueueMap        msg_queues_;
    unsigned int    max_msg_size_;
    unsigned int    max_nr_of_msgs_;

    dispatchTiDMessageCallback    disp_tid_msg_callback_ref_;
    boost::mutex                  shm_dispatch_mutex_;

};

}//namespace

#endif // TID_SHM_SERVER_H
