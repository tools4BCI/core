

#include <iostream>
#include <sstream>

#include "tid_client.h"
#include "sdl_tid_client.h"
#include "tid_exceptions.h"

#include "LptTools/LptTools.h"
#define LPT1  0
#define LPT2  1
#define LPT3  2
#define LPT4  3

#include <tobiid/IDSerializerRapid.hpp>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace TiD;

bool running;
bool  lpt_flag_;

//-----------------------------------------------------------------------

void getMessages(TiDClientBase* client)
{
  std::vector<IDMessage> msgs;
  msgs.reserve(1024);
  while(running)
  {
    msgs.clear();
    client->getLastMessagesContexts(msgs);
    if(msgs.size())
      cout << "Received messages: " << msgs.size() << endl;

    IDMessage recv_message;
    IDSerializerRapid recv_serializer;
    for(unsigned int n = 0; n < msgs.size(); n++)
    {
      cout << "Msg Nr: " << n  << endl;
      recv_serializer.SetMessage(&msgs[n]);
      msgs[n].Dump();


      int port_state = LptPortIn(LPT4,0);
      if(!lpt_flag_)
      {
        lpt_flag_ = 1;
        LptPortOut(LPT4, 0, port_state | 0xFF);
      }
      else
      {
        lpt_flag_ = 0;
        LptPortOut(LPT4, 0, port_state & ~0xFF);
      }

    }
    boost::this_thread::sleep(boost::posix_time::microsec(1));
  }
}

//-----------------------------------------------------------------------

int main()
{
  TiDClientBase* client = 0;
  boost::thread* recv_thread = 0;

  try
  {

    if(!LptDriverInstall())
    {
      cerr << "Installing LptTools lpt driver failed (do you have access rights for the lpt-port?)." << endl;
      throw std::runtime_error("Error installing LptTools lpt driver!");
    }

    if(!LptInit())
    {
      cerr << "Initializing lpt driver failed (do you have access rights for the lpt-port?)." << endl;
      throw std::runtime_error("Error initializing lpt driver!");
    }

    client = new TiDClient;


    client->connect(std::string("127.0.0.1"), 9100);
    client->startReceiving(false);

    string str;
    cout << endl << ">>";

    running = 0;

    while(cin >> str)
    {
      if(str == "q" || str == "quit" || str == "exit")
      {
        running = 0;
        if(recv_thread)
        {
          recv_thread->interrupt();
          recv_thread->join();
          delete recv_thread;
          recv_thread = 0;
        }

        break;
      }
      else if(str == "r" )
      {
        running = 1;
        recv_thread = new boost::thread(boost::bind(&getMessages, client) );

        cout << ">> " << endl;
      }
      else
        cout << "Command '" << str << "' not recognized!" << endl << ">>";
    }

    client->stopReceiving();
  }
  catch(std::exception& e)
  {
    if(recv_thread)
    {
      recv_thread->interrupt();
      delete recv_thread;
      recv_thread = 0;
    }

    std::cerr << "Exception caugth: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cerr << "Unknown exception caugth!" << std::endl;
  }

    delete client;

  return 0;
}

//-----------------------------------------------------------------------
