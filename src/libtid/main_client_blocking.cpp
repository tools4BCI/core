

#include <iostream>
#include <sstream>
#include <signal.h>

#include "sdl_tid_client.h"
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
  int srv_port = 9500;

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
    client = new SDLTiDClient;
    client->connect(srv_addr, srv_port);

    while(forever)
    {
      IDMessage msg = client->wait4NewTiDMessage();
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
