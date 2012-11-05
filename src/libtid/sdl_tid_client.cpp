#include "sdl_tid_client.h"

#include <boost/bind.hpp>

namespace TiD
{

//-----------------------------------------------------------------------------

SDLTiDClient::SDLTiDClient()
  : TiDClientBase(),
    receive_thread_(0), io_service_thread_(0), io_service_thread_2_(0)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

}

//-----------------------------------------------------------------------------

SDLTiDClient::~SDLTiDClient()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

    if(io_service_thread_)
    {
      SDL_KillThread(io_service_thread_);
      io_service_thread_ = 0;
    }

    if(io_service_thread_2_)
    {
      SDL_KillThread(io_service_thread_2_);
      io_service_thread_2_ = 0;
    }

    if(receive_thread_)
    {
      SDL_KillThread(receive_thread_);
      receive_thread_ = 0;
    }
}

//-----------------------------------------------------------------------------

void SDLTiDClient::startReceiving(bool throw_on_error)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  stopReceiving();

  throw_on_error_ = throw_on_error;

  if(state_ != State_Connected )
    if( state_ != State_Stopped )
      throw(std::runtime_error("SDLTiDClient::startReceiving -- not connected!") );

  state_ = State_Running;

  receive_thread_ = SDL_CreateThread(&TiDClientBase::receive, this);
  if(!receive_thread_)
    throw(std::runtime_error("SDLTiDClient::startReceiving -- Error creating receive-thread!") );

  io_service_thread_ = SDL_CreateThread(run_io_service, this);
  if(!io_service_thread_)
    throw(std::runtime_error("SDLTiDClient::startReceiving -- Error creating io_service-thread Nr. 1!") );

  io_service_thread_2_ = SDL_CreateThread(run_io_service, this);
  if(!io_service_thread_2_)
    throw(std::runtime_error("SDLTiDClient::startReceiving -- Error creating io_service-thread Nr. 2!") );


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

void SDLTiDClient::stopReceiving()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(!socket_.is_open())
    return;

  if(state_ != State_Running)
    return;

  state_ = State_Stopped;
  boost::system::error_code error;
  socket_.cancel(error);
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);

  io_service_.stop();

  if(io_service_thread_)
  {

    SDL_WaitThread(io_service_thread_, NULL);
    // delete io_service_thread_;
    io_service_thread_ = 0;
  }

  if(io_service_thread_2_)
  {
    SDL_WaitThread(io_service_thread_2_, NULL);
    // delete io_service_thread_2_;
    io_service_thread_2_ = 0;
  }


  if(receive_thread_)
  {
    //receive_thread_->interrupt();
    SDL_WaitThread(receive_thread_, NULL);
    // delete receive_thread_;
    receive_thread_ = 0;
  }

}

//-----------------------------------------------------------------------------

int SDLTiDClient::run_io_service(void* instance)
{
  SDLTiDClient* i = static_cast<SDLTiDClient*>(instance);
  i->io_service_.run();
  return 0;
}

//-----------------------------------------------------------------------------

}  //TiD
