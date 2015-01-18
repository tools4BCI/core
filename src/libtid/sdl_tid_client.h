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
    //static int SDLreceive(void *instance);

  private:
    SDL_Thread*                                       receive_thread_;
    SDL_Thread*                                       io_service_thread_;
    SDL_Thread*                                       io_service_thread_2_;
};

//-----------------------------------------------------------------------------

}  //TiD

#endif // SDL_TID_CLIENT_H
