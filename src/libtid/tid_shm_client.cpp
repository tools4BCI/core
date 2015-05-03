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

#include "tid_shm_client.h"

#include <utility>
#include <thread>
#include <boost/thread.hpp>
#include <boost/current_function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <tobiid/IDMessage.hpp>
#include <tobiid/IDSerializerRapid.hpp>

#include "messages/tid_message_builder_1_0.h"

//-----------------------------------------------------------------------------

namespace TiD
{

using std::make_pair;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

using namespace boost::interprocess;

TiDSHMClient::TiDSHMClient()
  : max_msg_size_(32768), max_nr_of_msgs_(200),
    mq_r_(make_pair("", (message_queue*)NULL)),
    mq_s_(make_pair("", (message_queue*)NULL)), msg_thread_(NULL)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  msg_builder_  = new TiDMessageBuilder10();
  xml_string_.resize(max_msg_size_);
}

//-----------------------------------------------------------------------------

TiDSHMClient::~TiDSHMClient()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(msg_builder_)
  {
    delete msg_builder_;
    msg_builder_ = 0;
  }

  try
  {
    closeMsgQueue();
  }
  catch(...)
  {

  }
}

//-----------------------------------------------------------------------------

void TiDSHMClient::setMaxMsgSize(unsigned int size)
{
  max_msg_size_ = size;
}

//-----------------------------------------------------------------------------

void TiDSHMClient::setMaxNrOfMsgs(unsigned int nr_msgs)
{
  max_nr_of_msgs_ = nr_msgs;
}

//-----------------------------------------------------------------------------

void TiDSHMClient::startReceivingFromSHM()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  try
  {
    if(!mq_r_.first.empty())
    {
      msg_thread_ = new boost::thread(boost::bind(&TiDSHMClient::receive, this));
    }
    else
      cerr << "Error: Message Queue '"+mq_r_.first+"' already in opened!" << endl;
  }
  catch(interprocess_exception &ex)
  {
    #ifdef DEBUG
      cerr << ex.what() << endl;
    #endif
  }
}

//-----------------------------------------------------------------------------

void TiDSHMClient::stopReceivingFromSHM()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(!msg_thread_)
    return;

  msg_thread_->interrupt();
  msg_thread_->join();

}

//-----------------------------------------------------------------------------

void TiDSHMClient::receive()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  IDSerializerRapid serializer;
  IDMessage msg;
  serializer.SetMessage(&msg);

  std::string buffer;
  buffer.resize(max_msg_size_);
  size_t msg_size = 0;
  unsigned int msg_prio = 0;

  boost::posix_time::ptime pt;


  while(1)
  {
    buffer.resize(max_msg_size_);
    try
    {
      //cerr << "trying to read from queue... "  <<  pt <<  endl;
      //pt = boost::posix_time::microsec_clock::universal_time()+ boost::posix_time::milliseconds(5);
      if(!mq_r_.second->try_receive(&buffer[0], buffer.size(), msg_size, msg_prio))
      //if(!mq_r_.second->timed_receive(&buffer[0], buffer.size(), msg_size, msg_prio, pt))
      {
        boost::this_thread::sleep(boost::posix_time::microseconds(5));
        boost::this_thread::interruption_point();
        continue;
      }

      boost::this_thread::disable_interruption di;
      buffer.resize(msg_size);

      serializer.Deserialize(&buffer);
      //msg.Dump(); cout << endl;

      shm_mutex_.lock();
      messages_from_shm_.push_back(msg);
      shm_mutex_.unlock();

      receiveSHMFinalHook();

    }
    catch(interprocess_exception &ex)
    {
      #ifdef DEBUG
        std::cerr << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION  << " -- Exception: " << ex.what() << std::endl;
      #endif
    }

    boost::this_thread::interruption_point();
  }
}

//-----------------------------------------------------------------------------

void TiDSHMClient::createSHMMessageQueue(const string& name)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  try
  {
    if(mq_r_.first.empty())
    {
      string name_r= name + "_r";
      mq_r_.second = new message_queue(open_or_create, name_r.c_str(), max_nr_of_msgs_, max_msg_size_);
      mq_r_.first = name;

      string name_s = name + "_s";
      mq_s_.second = new message_queue(open_or_create, name_s.c_str(), max_nr_of_msgs_, max_msg_size_);
      mq_s_.first = name;
    }
    else
      std::cerr << "Error: Message Queue '"+mq_r_.first+"' already in opened!" << std::endl;
  }
  catch(interprocess_exception &ex)
  {
    #ifdef DEBUG
      std::cerr << ex.what() << std::endl;
    #endif

    throw;
  }
}

//-----------------------------------------------------------------------------

void TiDSHMClient::closeMsgQueue()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  try
  {
    if(!mq_r_.first.empty())
    {
      message_queue::remove((mq_r_.first + "_r").c_str());
      delete mq_r_.second;
      mq_r_.second = NULL;

      message_queue::remove((mq_s_.first + "_s").c_str());
      delete mq_s_.second;
      mq_s_.second = NULL;
    }
    else
      cerr << "Error: Message Queue '"+mq_r_.first+"' not available!" << endl;
  }
  catch(interprocess_exception &ex)
  {
    #ifdef DEBUG
      cerr << ex.what() << endl;
    #endif
  }
}

//-----------------------------------------------------------------------------

void TiDSHMClient::sendMessage(string &tid_xml_context)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  try
  {
    mq_s_.second->send(tid_xml_context.data(), tid_xml_context.size(), 0);
  }
  catch(interprocess_exception &ex)
  {
    cerr << ex.what() << endl;
  }
}

//-----------------------------------------------------------------------------

void TiDSHMClient::sendMessage(IDMessage &msg)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << " -- " << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  msg_builder_->buildTiDMessage(msg, xml_string_);

  try
  {
    mq_s_.second->send(xml_string_.data(), xml_string_.size(), 0);
  }
  catch(interprocess_exception &ex)
  {
    cerr << ex.what() << endl;
  }

  xml_string_.clear();
}

//-----------------------------------------------------------------------------

bool TiDSHMClient::newMessagesAvailable()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  bool available = false;
  shm_mutex_.lock();
  available = messages_from_shm_.size();
  shm_mutex_.unlock();

  return available;
}

//-----------------------------------------------------------------------------

void TiDSHMClient::getLastMessagesContexts(std::vector<IDMessage> &messages)
{
  shm_mutex_.lock();
  messages.insert(messages.end(), messages_from_shm_.begin(), messages_from_shm_.end());
  messages_from_shm_.clear();
  shm_mutex_.unlock();
}

//-----------------------------------------------------------------------------

void TiDSHMClient::clearMessages()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  shm_mutex_.lock();
  messages_from_shm_.clear();
  shm_mutex_.unlock();
}

//-----------------------------------------------------------------------------

}
