#ifndef SDL_TID_CLIENT_H
#define SDL_TID_CLIENT_H

#include "tid_client_base.h"

#include <SDL/SDL_thread.h>

//-----------------------------------------------------------------------------

namespace TiD
{

class SDLTiDClient : public TiDClientBase
{
  public:
    SDLTiDClient();
    virtual ~SDLTiDClient();

    virtual void startReceiving( bool throw_on_error = 0 );
    virtual void stopReceiving();

  private:
    static int run_io_service(void* instance);

  private:
    SDL_Thread*                                       receive_thread_;
    SDL_Thread*                                       io_service_thread_;
    SDL_Thread*                                       io_service_thread_2_;

};

//-----------------------------------------------------------------------------

}  //TiD

#endif // SDL_TID_CLIENT_H
