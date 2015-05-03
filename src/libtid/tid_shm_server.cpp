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

#include "tid_shm_server.h"

#include <utility>
#include <thread>
#include <boost/thread.hpp>
#include <boost/current_function.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>

namespace TiD
{

using std::make_pair;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

using namespace boost::interprocess;

//-----------------------------------------------------------------------------

TiDSHMServer::TiDSHMServer(dispatchTiDMessageCallback disp_tid_msg_callback_ref)
  : max_msg_size_(32768), max_nr_of_msgs_(200),
    disp_tid_msg_callback_ref_(disp_tid_msg_callback_ref)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif
}

//-----------------------------------------------------------------------------

TiDSHMServer::~TiDSHMServer()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif



  QueueMap::iterator it;

  try
  {
    for(it = msg_queues_.begin(); it != msg_queues_.end(); it++)
    {
      closeMsgQueue(it->first);
    }
  }
  catch(...)
  {
    std::cerr << "Some Exception caught: " << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  }

  msg_queues_.clear();
}

//-----------------------------------------------------------------------------

void TiDSHMServer::setMaxMsgSize(unsigned int size)
{
  max_msg_size_ = size;
}

//-----------------------------------------------------------------------------

void TiDSHMServer::setMaxNrOfMsgs(unsigned int nr_msgs)
{
  max_nr_of_msgs_ = nr_msgs;
}

//-----------------------------------------------------------------------------

void TiDSHMServer::addSHMMessageQueue(const std::string& name)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  try
  {
    if(msg_queues_.find(name) == msg_queues_.end())
    {
      //oposite naming that client
      string name_r = name + "_s";
      message_queue* mq_r = new message_queue(open_only, name_r.c_str());

      string name_s = name + "_r";
      message_queue* mq_s = new message_queue(open_only, name_s.c_str());

      // first is for receiving  ... second is for sending
      msg_queues_.insert(make_pair(name, make_pair( make_pair(mq_r,mq_s), (boost::thread*)NULL )));
    }
    else
      std::cerr << "Error: Message Queue '"+name+"' already existing!" << std::endl;
  }
  catch(interprocess_exception &ex)
  {
    #ifdef DEBUG
      std::cerr << ex.what() << std::endl;
    #endif
    msg_queues_.insert( make_pair(name,
                  make_pair(make_pair((message_queue*)NULL,(message_queue*)NULL), (boost::thread*)NULL )) );
  }
}

//-----------------------------------------------------------------------------

void TiDSHMServer::closeMsgQueue(const std::string& name)
{

  QueueMap::iterator it;

  for(it = msg_queues_.begin(); it != msg_queues_.end(); it++)
  {
    if(it->first == name)
    {
      stopMessageQueueProcessing(name);

      if(it->second.first.first != NULL)
      {
        it->second.first.first->remove((it->first +"_s").c_str());
        delete it->second.first.first;
        it->second.first.first = NULL;
      }
      if(it->second.first.second != NULL)
      {
        it->second.first.second->remove((it->first +"_r").c_str());
        delete it->second.first.second;
        it->second.first.second = NULL;
      }
    }
    return;
  }
}

//-----------------------------------------------------------------------------

void TiDSHMServer::closeAllMsgQueues()
{

  QueueMap::iterator it;

  for(it = msg_queues_.begin(); it != msg_queues_.end(); it++)
  {
    string name = it->first;

    stopMessageQueueProcessing(name);

    if(it->second.first.first != NULL)
    {
      it->second.first.first->remove((it->first +"_s").c_str());
      delete it->second.first.first;
      it->second.first.first = NULL;
    }
    if(it->second.first.second != NULL)
    {
      it->second.first.second->remove((it->first +"_r").c_str());
      delete it->second.first.second;
      it->second.first.second = NULL;
    }

  }
}

//-----------------------------------------------------------------------------

void TiDSHMServer::startMessageQueueProcessing(const string& name)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  QueueMap::iterator it = msg_queues_.find(name);

  if(it != msg_queues_.end())
  {
    it->second.second = new boost::thread(boost::bind(&TiDSHMServer::receive, this, it));
  }
  else
    cerr << "Error: Message Queue '"+name+"' not existing!" << endl;
}

//-----------------------------------------------------------------------------

void TiDSHMServer::stopMessageQueueProcessing(const string& name)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  QueueMap::iterator it = msg_queues_.find(name);
  if(it != msg_queues_.end())
  {
    boost::thread* th = it->second.second;
    if(th != NULL)
    {
      th->interrupt();
      th->join();
      delete it->second.second;
      it->second.second = NULL;
    }
  }
  else
    cerr << "Error: Message Queue '"+name+"' not existing!" << endl;
}

//-----------------------------------------------------------------------------

void TiDSHMServer::stopMessageQueueProcessing()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  QueueMap::iterator it;

  for(it = msg_queues_.begin(); it != msg_queues_.end(); it++)
  {
    boost::thread* th = it->second.second;
    if(th != NULL)
    {
      th->interrupt();
      th->join();
      delete it->second.second;
      it->second.second = NULL;
    }
  }
}

//-----------------------------------------------------------------------------

void TiDSHMServer::receive(QueueMap::iterator it)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  const string name_r = it->first + "_s";
  const string name_s = it->first + "_r";
  message_queue** mq_r = &it->second.first.first;
  message_queue** mq_s = &it->second.first.second;

  std::pair<int, std::string> connection_id = make_pair(-99, name_r);

  IDSerializerRapid serializer;
  IDMessage msg;
  serializer.SetMessage(&msg);

  //cout << "Receiving on SHM: " << name_r << endl << std::flush;

  std::string buffer;
  buffer.resize(max_msg_size_);
  size_t msg_size = 0;
  unsigned int msg_prio = 0;

  while(1)
  {
    //try open queue
    if((*mq_r) == NULL)
    {
      try
      {
        *mq_r = new message_queue(open_only, name_r.c_str());
        *mq_s = new message_queue(open_only, name_s.c_str());
      }
      catch(interprocess_exception &ex)
      {
        #ifdef DEBUG
          std::cerr << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION  << " -- Exception: " << ex.what() << std::endl;
        #endif
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      }
    }
    else
    {
      buffer.resize(max_msg_size_);
      boost::posix_time::ptime pt =
                  boost::posix_time::microsec_clock::universal_time()+ boost::posix_time::milliseconds(250);
      try
      {
        //cerr << "trying to read from queue... "  <<  pt <<  endl;
        if((*mq_r)->timed_receive(&buffer[0], buffer.size(), msg_size, msg_prio, pt))
        {
          buffer.resize(msg_size);
          dispatchMsgToOtherQueues(buffer, it->first);

          serializer.Deserialize(&buffer);
          //msg.Dump(); cout << endl;

          disp_tid_msg_callback_ref_( msg, connection_id);
        }
        else
        {
          message_queue(open_only, name_r.c_str());
          message_queue(open_only, name_s.c_str());
        }
        boost::this_thread::interruption_point();
      }
      catch(interprocess_exception &ex)
      {
        #ifdef DEBUG
          std::cerr << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION  << " -- Exception: " << ex.what() << std::endl;
        #endif
        (*mq_r)->remove(name_r.c_str());
        delete *mq_r;
        *mq_r = NULL;

        (*mq_s)->remove(name_s.c_str());
        delete *mq_s;
        *mq_s = NULL;
      }
    }
  }

}

//-----------------------------------------------------------------------------

void TiDSHMServer::dispatchMsgToOtherQueues(string& msg, const string& origin)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  boost::mutex::scoped_lock(shm_dispatch_mutex_);
  QueueMap::iterator it;


  for(it = msg_queues_.begin(); it != msg_queues_.end(); it++)
  {
    if(it->first != origin)
    {
      message_queue* mq = it->second.first.second;
      if(mq != NULL)
      {
        //cout << "   ...  to: " << it->first << endl;
        if(!mq->try_send(msg.data(), msg.size(), 0))
          cerr << "Error sending message to: " << it->first << endl;
      }
    }
  }

}

//-----------------------------------------------------------------------------

}
