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

#ifndef TID_SHM_CLIENT_H
#define TID_SHM_CLIENT_H

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>


//-----------------------------------------------------------------------------

/**
* @class TiDSHMClient
*
* @brief TODO
*/

class IDMessage;

namespace TiD
{

class TiDMessageBuilder;

class TiDSHMClient
{
  friend class TimedTiDClient;

  public:
    TiDSHMClient();
    virtual ~TiDSHMClient();

    void createSHMMessageQueue(const std::string& name);
    void closeMsgQueue();

    void sendMessage(std::string& tid_xml_context);
    void sendMessage(IDMessage& msg);

    bool newMessagesAvailable();
    void getLastMessagesContexts( std::vector< IDMessage >& messages  );
    void clearMessages();

    void setMaxMsgSize(unsigned int size);
    void setMaxNrOfMsgs(unsigned int nr_msgs);

    void startReceivingFromSHM();
    void stopReceivingFromSHM();

  private:
    void receive();
    virtual void receiveSHMFinalHook() = 0;


  private:
    typedef std::pair< std::string, boost::interprocess::message_queue* > SHMQueue;

    unsigned int    max_msg_size_;
    unsigned int    max_nr_of_msgs_;

    std::string     xml_string_;

    SHMQueue                    mq_r_;
    SHMQueue                    mq_s_;
    boost::mutex                shm_mutex_;
    std::vector <IDMessage>     messages_from_shm_;
    boost::thread*              msg_thread_;
    TiDMessageBuilder*          msg_builder_;
};

}

#endif // TID_SHM_CLIENT_H
