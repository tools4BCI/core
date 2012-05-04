/*
    This file is part of the TOBI SignalServer test routine.

    The TOBI SignalServer test routine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The TOBI SignalServer test routine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the TOBI SignalServer test routine. If not, see <http://www.gnu.org/licenses/>.

    Copyright 2010 Christian Breitwieser
    Contact: c.breitwieser@tugraz.at
*/

#include "UnitTest++/UnitTest++.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

unsigned int NR_TID_MESSAGES = 50;
unsigned int NR_CLIENTS = 0;
unsigned int STATISTICS_WINDOW_SIZE = 5000;

#ifdef WIN32
  #include <windows.h>
#endif

//boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS(5);
//boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS(3);
boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS(2);
//boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS(1);

//boost::posix_time::microseconds SLEEP_TIME_BETWEEN_MSGS(500);
//boost::posix_time::microseconds SLEEP_TIME_BETWEEN_MSGS(200);
//boost::posix_time::microseconds SLEEP_TIME_BETWEEN_MSGS(100);

boost::posix_time::milliseconds SLEEP_TIME_BETWEEN_MSGS_REMOTE(4);

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

