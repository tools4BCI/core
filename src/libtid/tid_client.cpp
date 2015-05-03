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

#include "tid_client.h"
#include <thread>

namespace TiD
{

//-----------------------------------------------------------------------------

TiDClient::TiDClient()
  : TiDClientBase(),
    receive_thread_(0), io_service_thread_(0), io_service_thread_2_(0)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif


}

//-----------------------------------------------------------------------------

TiDClient::~TiDClient()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(io_service_thread_)
  {
    delete io_service_thread_;
    io_service_thread_ = 0;
  }

  if(io_service_thread_2_)
  {
    delete io_service_thread_2_;
    io_service_thread_2_ = 0;
  }

  if(receive_thread_)
  {
    receive_thread_->interrupt();
    delete receive_thread_;
    receive_thread_ = 0;
  }

}

//-----------------------------------------------------------------------------

void TiDClient::startReceiving(bool throw_on_error)
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  stopReceiving();

  throw_on_error_ = throw_on_error;

  if(state_ != State_Connected )
    if( state_ != State_Stopped )
      throw(std::runtime_error("TiDClient::startReceiving -- not connected!") );

  state_ = State_Running;
  receive_thread_ = new boost::thread(&TiDClientBase::receive, this);

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

void TiDClient::stopReceiving()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(!socket_->is_open())
    return;

  if(state_ != State_Running)
    return;

  state_ = State_Stopped;
  boost::system::error_code error;
  socket_->cancel(error);
  socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);

  io_service_->stop();
  if(io_service_thread_)
  {

    io_service_thread_->join();
    delete io_service_thread_;
    io_service_thread_ = 0;
  }
  if(io_service_thread_2_)
  {
    io_service_thread_2_->join();
    delete io_service_thread_2_;
    io_service_thread_2_ = 0;
  }

  if(receive_thread_)
  {
    receive_thread_->interrupt();
    #ifndef _WINDOWS
      receive_thread_->join();
    #endif
    delete receive_thread_;
    receive_thread_ = 0;
  }
}
//-----------------------------------------------------------------------------

void TiDClient::sendMessage(std::string &tid_xml_context)
{
  TiDClientBase::sendMessage(tid_xml_context);
  TiDSHMClient::sendMessage(tid_xml_context);
}

//-----------------------------------------------------------------------------

void TiDClient::sendMessage(IDMessage &msg)
{
  TiDClientBase::sendMessage(msg);
  TiDSHMClient::sendMessage(msg);
}

//-----------------------------------------------------------------------------

void TiDClient::getLastMessagesContexts(std::vector< IDMessage >& messages )
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  TiDClientBase::getLastMessagesContexts(messages);
  TiDSHMClient::getLastMessagesContexts(messages);
}

//-----------------------------------------------------------------------------

bool TiDClient::newMessagesAvailable()
{
  #ifdef DEBUG
    //std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif


  if(TiDClientBase::newMessagesAvailable())
    return true;
  if(TiDSHMClient::newMessagesAvailable())
    return true;

  return false;
}

//-----------------------------------------------------------------------------

void TiDClient::clearMessages()
{
  #ifdef DEBUG
    std::cout << std::this_thread::get_id() << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  TiDClientBase::clearMessages();
  TiDSHMClient::clearMessages();
}

//-----------------------------------------------------------------------------

}  //TiD
