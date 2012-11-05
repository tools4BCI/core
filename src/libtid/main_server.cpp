
#include <iostream>


#include "tid_server.h"

using namespace TiD;
using namespace std;

int main()
{
  try
  {
    TiDServer test_server;
    test_server.bind (9100);
    test_server.reserveNrOfMsgs(10000000);
    test_server.start();
    string str;
    cout << endl << ">>";

    while(cin >> str)
    {
        if(str == "q" || str == "quit" || str == "exit")
            break;
        else if(str == "r" )
        {
          std::vector<IDMessage> msgs;
          test_server.getLastMessages(msgs);

          std::cout << "Got " << msgs.size() << " messages" << std::endl;
        }
        else
            cout << "Command '" << str << "' not recognized!" << endl << ">>";
    }

    test_server.stop();
  }
  catch(std::exception& e)
  {
    cerr << "Exception caught: "<< e.what() <<  endl;
  }
  catch(...)
  {
    cerr << "Unknown exception caught!"<< endl;
  }

  return 0;
}
