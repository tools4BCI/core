/*
    This file is part of the libTiD test routine.

    The libTiD test routine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The libTiD test routine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the libTiD test routine. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#include "UnitTest++/UnitTest++.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/lexical_cast.hpp>
//#include <boost/chrono.hpp>
//#include <boost/date_time.hpp>
#include "timing_values.h"

unsigned int NR_TID_MESSAGES = 50;
unsigned int NR_CLIENTS = 0;
unsigned int STATISTICS_WINDOW_SIZE = 5000;

#ifdef WIN32
  #include <windows.h>
#endif




//-------------------------------------------------------------------------------------------------

int main( int argc, const char* argv[] )
{


  if(argc >1)
  {
    NR_TID_MESSAGES = boost::lexical_cast<unsigned int>(argv[1]);
  }

  if(argc > 2)
  {
    NR_CLIENTS = boost::lexical_cast<unsigned int>(argv[2]);
  }

  #ifdef WIN32
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
  #endif

    srand (time(NULL));
    std::cout << std::endl << "Build on " << __DATE__ << " at " << __TIME__ << std::endl;
    std::cout << "  ...  running " << NR_TID_MESSAGES << " messages on " << NR_CLIENTS << " clients ";
    std::cout << "with " << SLEEP_TIME_BETWEEN_MSGS << " between the messages." << std::endl<< std::endl;

    return UnitTest::RunAllTests();
}

//-------------------------------------------------------------------------------------------------

//#ifdef WIN32
//  SetPriorityClass( ptr->native_handle(), REALTIME_PRIORITY_CLASS);
//  SetThreadPriority(ptr->native_handle(), THREAD_PRIORITY_HIGHEST );
//  SetPriorityClass(GetCurrentProcess(),   REALTIME_PRIORITY_CLASS);
//#endif

//SKIP_TOBIID_SERIALIZE_TEST
//SKIP_TOBIID_DESERIALIZE_TEST
//SKIP_LIBTID_CLIENT_SEND_TEST
//SKIP_LIBTID_CLIENT_RECV_TEST
//SKIP_LIBTID_LPT_TEST
//SKIP_LIBTID_LOCALHOST_SEND_RECEIVE_TEST
//SKIP_LIBTID_REMOTE_SEND_RECEIVE_TEST
//SKIP_LIBTID_SERVER_DISPATCH_TEST
//SKIP_STREAM_PARSING_TEST

