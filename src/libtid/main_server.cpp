
#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "tid_server.h"

using namespace TiD;
using namespace std;

int main()
{
  boost::asio::io_service io;

  TiDServer test_server(io);


  test_server.bind (9001);
  test_server.listen ();

  boost::thread* io_service_thread_ = new boost::thread(boost::bind(&boost::asio::io_service::run, &io));

  string str;
  cout << endl << ">>";

  while(cin >> str)
  {
    if(str == "q" || str == "quit" || str == "exit")
      break;
    else
      cout << "Command '" << str << "' not recognized!" << endl << ">>";
  }

  io.stop();

  cout << " Joining ... " << endl;

  io_service_thread_->interrupt();
  io_service_thread_->join();

  cout << "   done ... " << endl;

  delete(io_service_thread_);
  cout << "   deleted ... " << endl;

  return 0;
}
