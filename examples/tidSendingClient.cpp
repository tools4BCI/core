
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

#include <iostream>

#include "libtid/tid_client.h"
#include "tobiid/IDMessage.hpp"
#include "libtid/tid_exceptions.h"

using namespace std;
using namespace TiD;
bool forever = true;


//---------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
  std::string srv_addr("127.0.0.1");
  int srv_port = 9001;
  TiDClient* client;

  try
  {
    client = new TiDClient;
    client->connect(srv_addr, srv_port);

    int count = 0;
    while(forever)
    {
      cout << "Sleeping for 0.5s" << endl;
      usleep(500000);
      IDMessage msg;
      msg.SetEvent(count++);
      msg.SetValue(count*1.4);
      msg.SetFamily("custom");
      msg.SetSource("local");

      client->sendMessage(msg);
    }

  }
  catch(TiDException& e)
  {
    std::cerr << "TiDException caugth: " << e.what() << std::endl;
  }
  catch(std::exception& e)
  {
    std::cerr << "Exception caugth: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cerr << "Unknown exception caugth!" << std::endl;
  }

  if(client)
    delete client;

  return 0;
}

