#include "sdl_tid_client.h"

#include <boost/bind.hpp>
#include <iostream>
#include <fstream>

#include "input_stream_socket.h"
#include "messages/tid_message_parser_1_0.h"
#include "messages/tid_message_builder_1_0.h"

#include <SDL/SDL.h>

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

  //  SDL_Init( SDL_INIT_EVENTTHREAD );
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

  //std::cout << BOOST_CURRENT_FUNCTION << " --- nearly" <<  std::endl;

  if(receive_thread_)
  {
    SDL_KillThread(receive_thread_);
    receive_thread_ = 0;
  }

  //    SDL_Quit();
}

//-----------------------------------------------------------------------------

void SDLTiDClient::startReceiving(bool throw_on_error)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl << std::flush;
  #endif

  stopReceiving();

  throw_on_error_ = throw_on_error;

  if(state_ != State_Connected )
    if( state_ != State_Stopped )
      throw(std::runtime_error("SDLTiDClient::startReceiving -- not connected!") );

  state_mutex_.lock();
  state_ = State_Running;
  state_mutex_.unlock();

  receive_thread_ = SDL_CreateThread(&TiDClientBase::receive, this);
  if(!receive_thread_)
    throw(std::runtime_error("SDLTiDClient::startReceiving -- Error creating receive-thread!") );

  io_service_thread_ = SDL_CreateThread(run_io_service, this);
  if(!io_service_thread_)
    throw(std::runtime_error("SDLTiDClient::startReceiving -- Error creating io_service-thread Nr. 1!") );

  io_service_thread_2_ = SDL_CreateThread(run_io_service, this);
  if(!io_service_thread_2_)
    throw(std::runtime_error("SDLTiDClient::startReceiving -- Error creating io_service-thread Nr. 2!") );
}

//-----------------------------------------------------------------------------

void SDLTiDClient::stopReceiving()
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
  #endif

  if(!socket_->is_open())
    return;

  if(state_ != State_Running)
    return;

  state_mutex_.lock();
  state_ = State_Stopped;
  state_mutex_.unlock();
  boost::system::error_code error;
  socket_->cancel(error);
  socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);

  if(!io_service_->stopped())
    io_service_->stop();

  if(io_service_thread_)
  {
    SDL_WaitThread(io_service_thread_, NULL);
    io_service_thread_ = 0;
  }

  if(io_service_thread_2_)
  {
    SDL_WaitThread(io_service_thread_2_,NULL);
    io_service_thread_2_ = 0;
  }


  if(receive_thread_)
  {
    SDL_WaitThread(receive_thread_, NULL);
//    SDL_KillThread(receive_thread_);
    receive_thread_ = 0;
  }

}

//-----------------------------------------------------------------------------

int SDLTiDClient::run_io_service(void* instance)
{
  #ifdef DEBUG
    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl << std::flush;
  #endif

  SDLTiDClient* i = static_cast<SDLTiDClient*>(instance);
  i->io_service_->run();
  return 0;
}

//-----------------------------------------------------------------------------

//int SDLTiDClient::SDLreceive(void* instance)
//{
//  #ifdef DEBUG
//    std::cout << BOOST_CURRENT_FUNCTION <<  std::endl;
//  #endif

//  SDLTiDClient* inst = static_cast<SDLTiDClient*> (instance);

//  ConnectionState tmp_state;
//  inst->state_mutex_.lock();
//  tmp_state = inst->state_;
//  inst->state_mutex_.unlock();

//  while(tmp_state == State_Running)
//  {
//    inst->state_mutex_.lock();
//    tmp_state = inst->state_;
//    inst->state_mutex_.unlock();

//    try
//    {
//      IDMessage msg;
//      inst->msg_parser_->parseMessage(&msg, inst->input_stream_ );
//      inst->mutex_.lock();
//      inst->messages_.push_back(msg);
//      inst->mutex_.unlock();
//    }
//    catch(TiDLostConnection&)
//    {
//      //      stopReceiving();
//      std::cerr << "   ***  Connection to TiD Server@" <<
//        inst->remote_ip_ << ":" << inst->remote_port_ << " lost." << std::endl << " >> ";
//      break;
//    }
//    catch(TiDException& e)
//    {
//      if(tmp_state == State_Running)
//        std::cerr << e.what() << std::endl << ">> ";
//        //      stopReceiving();
//      break;
//    }
//    catch(std::exception& e)
//    {
//      //      if(inst->throw_on_error_)
//      //        boost::throw_exception(e);

//      if(tmp_state == State_Running)
//        std::cerr << e.what() << std::endl << ">> ";
//      inst->state_mutex_.lock();
//      inst->state_ = State_Error;
//      inst->state_mutex_.unlock();
//      return 1;
//    }
//  }
//  //inst->state_ = State_Error;

//  return 0;
//}


//-----------------------------------------------------------------------------



}  //TiD
