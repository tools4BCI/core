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
#include <sstream>
#include <signal.h>

#include "tid_client.h"
#include "tid_exceptions.h"
#include <boost/chrono.hpp>

#include <tobiid/IDSerializerRapid.hpp>

using namespace std;
using namespace TiD;
bool forever = true;
TiDClientBase* client = 0;

//---------------------------------------------------------------------------------------

void sighandler(int sig)
{
    cout<< "Signal " << sig << " caught..." << endl;

  forever = false;
  client->disconnect();
}

//---------------------------------------------------------------------------------------

int main(int argc, const char* argv[])
{
  signal(SIGABRT, &sighandler);
  signal(SIGTERM, &sighandler);
  signal(SIGINT, &sighandler);



  std::string srv_addr("127.0.0.1");
  int srv_port = 9001;

  if(argc == 1)
  {
    cout << "Using default server " << srv_addr << ":" << srv_port << endl;
  }
  else if(argc == 3)
  {
    srv_addr = argv[1];
    stringstream conv(argv[2]);
    conv >> srv_port;
    cout << "Using server " << srv_addr << ":" << srv_port << endl;
  }

  try
  {
    client = new TiDClient;
    client->connect(srv_addr, srv_port);

    while(forever)
    {
      IDMessage msg = client->wait4NewTiDMessage();
      std::cout << "Value: " << msg.GetValue() << std::endl;
      msg.Dump();
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

  delete client;
  return 0;
}

//---------------------------------------------------------------------------------------
